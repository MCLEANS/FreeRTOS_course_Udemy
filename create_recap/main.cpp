#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Task handles
 */
TaskHandle_t red_task_handle;
TaskHandle_t blue_task_handle;
TaskHandle_t orange_task_handle;
TaskHandle_t green_task_handle;


/**
 * System Tasks
 */
void red_task(void* pvParam){
  while(1){

  }
}

void blue_task(void* pvParam){
  while(1){

  }
}

void orange_task(void* pvParam){
  while(1){

  }
}

void green_task(void* pvParam){
  while(1){

  }
}


int main(void) {
  
  system_clock.initialize();
  //configure LEDs as output
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  /**
   * Create system tasks
   */
  xTaskCreate(red_task,
              "Red Led Controller",
              100,
              NULL,
              1,
              &red_task_handle);
  xTaskCreate(blue_task,
              "Blue Task Controller",
              100,
              NULL,
              1,
              &blue_task_handle);
  xTaskCreate(orange_task,
              "Orange Task Controller",
              100,
              NULL,
              1,
              &orange_task_handle);
  xTaskCreate(green_task,
              "Green task Controller",
              100,
              NULL,
              1,
              &green_task_handle);
              
  vTaskStartScheduler();

  while(1){

  }
}
