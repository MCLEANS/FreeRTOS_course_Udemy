#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "LIS3DH.h"
#include "NOKIA_5110.h"
#include "EXTI.h"

#include <queue.h>
#include <semphr.h>


/**
 * 1. Get accelerometer values from one task and push the values to LCD display task
 * via a queue.
 * 2. Use a semaphore to toggle an LED each time a valid accelerometer value is read.
 */

#define ANGLE_VALUES_QUEUE_LENGTH 20
#define MS_TO_WAIT_ANGLES_VALUES_QUEUE_SEND 0
#define MS_TO_WAIT_ANGLES_VALUES_QUEUE_RECEIVE 0
#define SCREEN_REFRESH_RATE 100 //Representation of 60Hz in ms

custom_libraries::clock_config system_clock;

/**
 * LED object initialization
 */
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Accelerometer object Initialization
 */
#define SCK_PIN 5
#define MOSI_PIN 7
#define MISO_PIN 6

#define CS_PORT GPIOE
#define CS_PIN 3

/**
 * Accelerometer initialization
 */
custom_libraries::LIS3DH motion_sensor(SPI1,
                                        GPIOA,
                                        SCK_PIN,
                                        MOSI_PIN,
                                        MISO_PIN,
                                        CS_PORT,
                                        CS_PIN);

/**
 * Initialize NOKIA 5110 LCD
 */
#define NOKIA_RST_PORT GPIOD
#define NOKIA_RST_PIN 0
#define NOKIA_CS_PORT GPIOD
#define NOKIA_CS_PIN 1
#define NOKIA_DC_PORT GPIOD
#define NOKIA_DC_PIN 2

custom_libraries::NOKIA_5110 NOKIA(SPI2,
                                    GPIOB,
                                    13,
                                    15,
                                    0,
                                    64,
                                    false,
                                    false,
                                    false,
                                    NOKIA_CS_PORT,
                                    NOKIA_CS_PIN,
                                    NOKIA_RST_PORT,
                                    NOKIA_RST_PIN,
                                    NOKIA_DC_PORT,
                                    NOKIA_DC_PIN);

/**
 * Control button initialization
 */
custom_libraries::_EXTI control_button(GPIOA,0,custom_libraries::FALLING);

/**
 * System task handles
 */
TaskHandle_t red_indicator_task;
TaskHandle_t blue_inidator_task;
TaskHandle_t orange_indicator_task;
TaskHandle_t green_indicator_task;
TaskHandle_t accelerometer_task;
TaskHandle_t display_task;
TaskHandle_t user_button_task;

/**
 * Status variables
 */
bool is_initialized_accel = false;
bool accel_values_received_by_display = false;
bool accel_values_sent_to_display = false;
bool is_initialized_angles_queue = false;

/**
 * Enum to hold display pages
 */
enum Display_page{
  status,
  values
};

Display_page current_page = status;

/**
 * System Queue handles
 */
QueueHandle_t angle_values_queue;

char image[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xef, 0x75, 0xf5, 0xcf, 0xef, 0xdb, 0x37, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x01, 0x00, 0xc0, 0x01, 0x02, 0x83, 0x04, 
0x07, 0x03, 0x00, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0xbf, 0x1f, 0x1f, 0x1f, 0x3f, 0x3e, 0x9c, 0x60, 
0x00, 0xf0, 0x31, 0x00, 0x00, 0x30, 0x1c, 0x0f, 0x8f, 0x8f, 0x5f, 0x1f, 0x1f, 0x3f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0f, 0x00, 0x04, 0x18, 0x20, 0x71, 0x61, 0x02, 
0x6a, 0xdd, 0xdb, 0xda, 0xff, 0x01, 0x00, 0x00, 0x12, 0x22, 0x31, 0x31, 0x00, 0x18, 0x18, 0x08, 
0x04, 0x06, 0x01, 0x8f, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xc9, 0x64, 0x98, 0xc8, 0x30, 0x90, 0x62, 
0x24, 0xc0, 0x44, 0x88, 0x8c, 0x08, 0x05, 0x05, 0x06, 0x0b, 0x02, 0x08, 0x0c, 0x0e, 0x02, 0x8c, 
0x86, 0x42, 0xc4, 0x26, 0x62, 0x90, 0x33, 0xc9, 0x98, 0x64, 0xc9, 0xbf, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xfe, 0xff, 0xfd, 0xfe, 0xfb, 0xfc, 0xf6, 0xf9, 0xec, 0xf3, 0xd9, 0xe6, 0xf2, 0xe4, 0xe4, 0xf2, 
0xe6, 0xd9, 0xf3, 0xec, 0xf9, 0xf6, 0xfc, 0xfb, 0xfe, 0xfd, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/**
 * Semaphore handles
 */
SemaphoreHandle_t button_press_semaphore;

void tostring(char str[], int num)
{
  int i, rem, len = 0, n;
  n = num;
  while (n != 0)
  {
    len++; //get length for string/digits in int
    n = n / 10;
  }
  //convert and store in string
  for (i = 0; i < len; i++)
  {
    rem = num % 10;                 //last digit fetched first
    num = num / 10;                 //continue fetching rest of the digits
    str[len - (i + 1)] = rem + '0'; //start storing string with max-1 index first
  }
  str[len] = '\0'; //null to end the string[max]
}

/**
 * System task declarations
 */
void red_indicator(void* pvParameter){
  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(100));

  }
}

void blue_indicator(void* pvParameter){
  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));

  }
}

void orange_indicator(void* pvParameter){
  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void green_indicator(void* pvParameter){
  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(150));
  }
}

void accelerometer_handler(void* pvParameter){
   /**
   * Initialize the motion sensor
   */
  bool Accel_is_initialized = motion_sensor.initialize();
  /**
   * Check whether the Motion sensor was properly Initialized
   */
  if(!Accel_is_initialized){
    /**
     * Perform error handling here
     */
    is_initialized_accel = false;
  }
  else{
    is_initialized_accel = true;
  }
  /**
   * Variable to hold received accelerometer values
   */
  custom_libraries::Angle_values angle_values;

  while(1){
    /**
     * Write tasks to handle obtaining data from the Accelerometer
     */
    angle_values = motion_sensor.read_angles();
    BaseType_t angle_values_send_successfull =  xQueueSend(angle_values_queue,
                                                          (void*)&angle_values,
                                                          pdMS_TO_TICKS(MS_TO_WAIT_ANGLES_VALUES_QUEUE_SEND));
    if(angle_values_send_successfull != pdTRUE){
      /**
       * Error handling here, Item was not successfully added to the queue
       */
      accel_values_sent_to_display = false;
    }
    else{
      accel_values_sent_to_display = true;
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void display_handler(void* pvParameter){
  /**
   * Configure display to nornmal mode
   */
  NOKIA.inverted_mode();
  /**
   * Display constants
   */

  for(volatile int i = 0; i < 508; i++){
    NOKIA.send_data(image[i]);
  }

  vTaskDelay(pdMS_TO_TICKS(4000));

  NOKIA.normal_mode();
  char clockwise[] = "CLK";
  char anticlockwise[] = "ACLK";
  char axis_x[] = "X";
  char axis_y[] = "Y";
  char page2_title[] = "STATUS";
  char accel_init[] = "AI";
  char accel_received_display[] = "ARD";
  char accel_sent_display[] = "ASD";

    /**
   * Character arrays to hold values to display
   */
  char x_axis_[5];
  char y_axis_[5];

  /**
   * variable to store received accelerometer values
   */
  custom_libraries::Angle_values angle_values;
  /**
   * Get current task wake time
   */
  TickType_t previous_wake_time = xTaskGetTickCount();

  while(1){
    /**
     * Reveive data from angle values queue
     */
    BaseType_t angle_values_received_sucessfully = xQueueReceive(angle_values_queue,
                                                                  &angle_values,
                                                                  pdMS_TO_TICKS(MS_TO_WAIT_ANGLES_VALUES_QUEUE_RECEIVE));
    if(angle_values_received_sucessfully != pdTRUE){
      /**
       * Perform error handling here, queue values was not sucessfully received
       */
      accel_values_received_by_display = false;
    }
    else{
      accel_values_received_by_display = true;
    }

    tostring(x_axis_,angle_values.x_axis);
    tostring(y_axis_,angle_values.y_axis);
    /**
     * Refresh the screen
     */
    NOKIA.clear();

    /**
     * handle which page to display on the LCD
     */
    if(current_page == status){
      /**
       * Display status info here
       */
      NOKIA.print(page2_title,20,0);
      NOKIA.print(accel_init,5,1);
      if(is_initialized_accel){
        NOKIA.mark_point(20,1);
      }
      else{
        NOKIA.unmark_point(20,1);
      }
      NOKIA.print(accel_received_display,5,3);
      if(accel_values_received_by_display){
        NOKIA.mark_point(25,3);
      }
      else{
        NOKIA.unmark_point(25,3);
      }
      NOKIA.print(accel_sent_display,5,5);
      if(accel_values_sent_to_display){
        NOKIA.mark_point(25,5);
      }
      else{
        NOKIA.unmark_point(25,5);
      }
   }

    else if(current_page == values){
      /**
       * Display accelerometer values here
       */
      NOKIA.print(clockwise,20,1);
      NOKIA.print(anticlockwise,55,1);
      NOKIA.print(axis_x,5,2);
      NOKIA.print(axis_y,5,4);

      if(angle_values.x_clockwise){
        NOKIA.print(x_axis_,20,2);
        NOKIA.mark_point(55,2);
        }
      else{
        NOKIA.mark_point(20,2);
        NOKIA.print(x_axis_,55,2);
      }

    if(angle_values.y_clockwise){
        NOKIA.print(y_axis_,20,4);
        NOKIA.mark_point(55,4);
      }
      else{
        NOKIA.print(y_axis_,55,4);
        NOKIA.mark_point(20,4);
       }  
      
    }

   vTaskDelayUntil(&previous_wake_time,
                  pdMS_TO_TICKS(SCREEN_REFRESH_RATE));
  }
}

void user_button_handler(void* pvParameter){
  while(1){
    if(xSemaphoreTake(button_press_semaphore,portMAX_DELAY) == pdTRUE){
       switch(current_page){
        case status:
          current_page = values;
          break;
        case values:
          current_page = status;
          break;
      }
    }
  }
}

/**
 * External Interrupt ISR
 */
extern "C" void EXTI0_IRQHandler(void){
  BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;
  if(EXTI->PR & EXTI_PR_PR0){
		EXTI->PR |= EXTI_PR_PR0;
		/**
     * Perform Sysnchronisation of interupt here
     */
    xSemaphoreGiveFromISR(button_press_semaphore,&xHigherPriorityTaskWoken);
	}
  if(xHigherPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
  
}

int main(void) {
  /**
   * Initialization of the system clock
   */
  system_clock.initialize();
  /**
   * Configure LED as output
   */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);
  /**
   * Configuration of LED output settings
   */
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  /**
   * Initialize control button
   */
  control_button.initialize();

  /**
   * Enable external Interrupt 0
   */
  NVIC_SetPriority(EXTI0_IRQn,0x05);
  NVIC_EnableIRQ(EXTI0_IRQn);

  /**
   * Create angle values queue
   */
  angle_values_queue =  xQueueCreate(ANGLE_VALUES_QUEUE_LENGTH,
                                    sizeof(custom_libraries::Angle_values));
  /**
   * Check if queue was created succesfully
   */
  if(angle_values_queue == NULL){
    /**
     * perform error handling here
     */
  }
  
  /**
   * Create button press semaphore
   */
  button_press_semaphore = xSemaphoreCreateBinary();
  /**
   * Check if semaphore was successfully created
   */  
  if(button_press_semaphore == NULL){
    /**
     * Perform error handling here
     */
  }
  else{
    xSemaphoreGive(button_press_semaphore);
  }

  /**
   * Create system tasks
   */
  xTaskCreate(red_indicator,
              "Red LED Indicator",
              100,
              NULL,
              1,
              &red_indicator_task);
  xTaskCreate(blue_indicator,
              "Blue LED Indicator",
              100,
              NULL,
              1,
              &blue_inidator_task);
  xTaskCreate(green_indicator,
              "Green task indicator",
              100,
              NULL,
              1,
              &green_indicator_task);
  xTaskCreate(orange_indicator,
              "Orange task idicator",
              100,
              NULL,
              1,
              &orange_indicator_task);
  xTaskCreate(accelerometer_handler,
              "Accelerometer task",
              100,
              NULL,
              1,
              &accelerometer_task);
  xTaskCreate(display_handler,
              "LCD Display task",
              100,
              NULL,
              2,
              &display_task);
  xTaskCreate(user_button_handler,
              "Task to handle the user button",
              100,
              NULL,
              1,
              &user_button_task);

  vTaskStartScheduler();

  while(1){

  }
}
