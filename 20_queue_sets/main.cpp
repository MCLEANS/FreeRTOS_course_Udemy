#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <queue.h>
#include <semphr.h>

#include "GPIO.h"
#include "LIS3DH.h"
#include "EXTI.h"

#define INTERRUPT_PORT GPIOA
#define INTERRUPT_PIN 0

#define ACCEL_QUEUE_LENGTH 10

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/* Create interrupt  edge type */
custom_libraries::edge response_edge = custom_libraries::RISING;

/* Create interrupt object */
custom_libraries::_EXTI halt_button(INTERRUPT_PORT,INTERRUPT_PIN,response_edge);

/* LIS3DH pin definition */
#define SCK_PIN 5
#define MOSI_PIN 7
#define MISO_PIN 6

#define CS_PORT GPIOE
#define CS_PIN 3

/* Create accelerometer object */
custom_libraries::LIS3DH accelerometer(SPI1,
                                      GPIOA,
                                      SCK_PIN,
                                      MOSI_PIN,
                                      MISO_PIN,
                                      CS_PORT,
                                      CS_PIN);

TaskHandle_t sensor_read_handle;
TaskHandle_t led_blink_handle;

xQueueHandle accel_value_queue;

SemaphoreHandle_t freeze_flag;

xQueueSetHandle blink_task_queue_set;

QueueSetMemberHandle_t activated_member;

/* External Interrupts ISR */
extern "C" void EXTI0_IRQHandler(void){
  /* verify interrupt, reset flag and give semaphore */
  if(EXTI->PR & EXTI_PR_PR0){
		EXTI->PR |= EXTI_PR_PR0;
    static BaseType_t xHigherPriorityTaskWoken;
    xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR( freeze_flag, &xHigherPriorityTaskWoken );
    if(xHigherPriorityTaskWoken == pdTRUE){
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  }
}

/* task to blink leds based on accel angles */
void led_blink_task(void *pvParam){

  /* Configure LEDs as output */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  bool freeze = false;
  custom_libraries::Angle_values angle_values;

  while(1){
    activated_member = xQueueSelectFromSet( blink_task_queue_set,0);
    if(activated_member == freeze_flag){
      if( xSemaphoreTake( freeze_flag, 0 ) == pdTRUE ){
        if(freeze){
          freeze = false;
        }
        else if(!freeze){
          freeze = true;
        }  
      }
    }
    if(activated_member == accel_value_queue){
        /* Start LED sequence */
        /* check if there is data available in queue and retreive */
        if (xQueueReceive(accel_value_queue, &angle_values, 0) == pdPASS)
        {
            if(!freeze){
              /* Accel values have been received successfully */
              if (angle_values.x_clockwise && angle_values.y_clockwise)
              {
                red_led.toggle();
                orange_led.toggle();
                blue_led.digital_write(0);
                green_led.digital_write(0);
              }

              else if (!angle_values.x_clockwise && angle_values.y_clockwise)
              {
                orange_led.toggle();
                green_led.toggle();
                red_led.digital_write(0);
                blue_led.digital_write(0);
              }

              else if (!angle_values.x_clockwise && !angle_values.y_clockwise)
              {
                green_led.toggle();
                blue_led.toggle();
                orange_led.digital_write(0);
                red_led.digital_write(0);
              }

              else if (angle_values.x_clockwise && !angle_values.y_clockwise)
              {
                red_led.toggle();
                blue_led.toggle();
                orange_led.digital_write(0);
                green_led.digital_write(0);
              }
            }
          else{
            orange_led.digital_write(0);
            green_led.digital_write(0);
            red_led.digital_write(0);
            blue_led.digital_write(0);
          }
        }
    }

    vTaskDelay(pdMS_TO_TICKS(50));   
  }
}

void sensor_read_task(void *pvParam){
  
  /* Initialize accelerometer */
  accelerometer.initialize();

  /* Variable to hold angles from accelerometer */
  custom_libraries::Angle_values accel_angles;

  while(1){
    /* read values from accelerometer */
    accel_angles = accelerometer.read_angles();

    /* send value into the accel value queue */
    if(xQueueSend(accel_value_queue,(void*)&accel_angles,portMAX_DELAY) != pdPASS){
      /* failed to add to queue, handle error */
    }

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

int main(void) {
  
  system_clock.initialize();

  /* Initialize interrupt pin */
  halt_button.initialize();

  /* Set interrupt vector and enable */
  NVIC_SetPriority(EXTI0_IRQn, 0x05);
  NVIC_EnableIRQ(EXTI0_IRQn);

  /* Create queue */
  accel_value_queue = xQueueCreate(ACCEL_QUEUE_LENGTH,sizeof(custom_libraries::Angle_values));
  if(accel_value_queue == NULL){
    /* Not enough heap to create queue, handle error */
  }

  /* Create freeze semaphore */
  freeze_flag = xSemaphoreCreateBinary();
  if(freeze_flag == NULL){
    /* Not enough heap to create semaphore, handle error */
  }
  else{
    //xSemaphoreGive(freeze_flag);
  }

  /* Create queue set */
  blink_task_queue_set = xQueueCreateSet((ACCEL_QUEUE_LENGTH + 1));

  /* Add queue and semaphore to queue set */
  xQueueAddToSet(accel_value_queue,blink_task_queue_set);
  xQueueAddToSet(freeze_flag,blink_task_queue_set);

  xTaskCreate(sensor_read_task,
              "Task to read accel data from sensor",
              100,
              NULL,
              1,
              &sensor_read_handle);
  
  xTaskCreate(led_blink_task,
              "Task to blink led based on accelerometer angles",
              100,
              NULL,
              1,
              &led_blink_handle);
  
  vTaskStartScheduler();

  while(1){

  }
}
