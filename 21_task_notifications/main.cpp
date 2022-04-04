#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "EXTI.h"

#define INTERRUPT_PORT GPIOA
#define INTERRUPT_PIN 0

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/* External interrupt specifications */
custom_libraries::edge response_edge = custom_libraries::RISING;
custom_libraries::_EXTI button(INTERRUPT_PORT,INTERRUPT_PIN,response_edge);

TaskHandle_t blink_task_handle;

extern "C" void EXTI0_IRQHandler(void){
  if(EXTI->PR & EXTI_PR_PR0){
    /* We are here because of Interrupt on PAO */
    EXTI->PR |= EXTI_PR_PR0;
    /* Send notification to blinking task */
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(blink_task_handle, &xHigherPriorityTaskWoken);

    if(xHigherPriorityTaskWoken == pdTRUE){
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  }
}

void blinky(void* pvParam){
  bool led_blink = true;

  while(1){

    if(ulTaskNotifyTake(pdFALSE, portMAX_DELAY) != 0){
      red_led.toggle();
      blue_led.toggle();
      orange_led.toggle();
      green_led.toggle();
    }
    
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

int main(void) {
  
  system_clock.initialize();
  /* Initialize external interrupts */
  button.initialize();
  /* Enable hardware interrupts */
  NVIC_SetPriority(EXTI0_IRQn,0x05);
  NVIC_EnableIRQ(EXTI0_IRQn);

  //configure LEDs as output
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  xTaskCreate(blinky,
              "Task to blink LEDs",
              100,
              NULL,
              1,
              &blink_task_handle);
  
  vTaskStartScheduler();

  while(1){

  }
}
