#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/***************************************************************************
 * 1. The scheduler gives control to the highest priority unblocked task.
 * 2. When two tasks are of the same priority a round robin scheduling occurs giving each equal access to the CPU.
 * 3. To interract with tasks at runtime we need a handle;
 * ******************************************************************************/

/***********************************************************************************
 * Here we use the green_led_task to change the priority of the blue_led_task
 * ********************************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

TaskHandle_t blue_led_handle;
TaskHandle_t red_led_handle;
TaskHandle_t green_led_handle;
TaskHandle_t orange_led_handle;

void green_led_task(void* pvParameter){

  while(1){
    for(int i = 0; i < 5000000; i++){}
    green_led.toggle();
    //change the priority of the blue_led_task making it the highest priority
    vTaskPrioritySet(blue_led_handle,3);

  }
}

void orange_led_task(void* pvParameter){

  while(1){
    for(int i = 0; i < 5000000; i++){}
    orange_led.toggle();
  }
}

void red_led_task(void* pvParameter){

  while(1){
    for(int i = 0; i < 5000000; i++){}
    red_led.toggle();
  }
}

void blue_led_task(void* pvParameter){

  while(1){
    for(int i = 0; i < 5000000; i++){}
    blue_led.toggle();
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

  xTaskCreate(green_led_task,
              "Green led cotroller",
              100,
              NULL,
              2,
              &green_led_handle);
  xTaskCreate(orange_led_task,
              "orange led cotroller",
              100,
              NULL,
              1,
              &orange_led_handle);
  xTaskCreate(red_led_task,
              "red led cotroller",
              100,
              NULL,
              2,
              &red_led_handle);
  xTaskCreate(blue_led_task,
              "blue led cotroller",
              100,
              NULL,
              1,
              &blue_led_handle);
  
  vTaskStartScheduler();

  while(1){

  }
}