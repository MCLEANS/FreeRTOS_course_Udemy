#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/**
 * 1. When the kernel is configured as completely pre-emptive it will always run the task with the highest priority unblocked state.
 *    When two tasks are of the same priority there will be no time sharing between the tasks but the task that was created first will be allowed
 *    to run without pre-emption form another task with the same priority
 * 
 * 2. We can use RTOS delays to create a pseudo time sharing scheme by blocking tasks. This type of time sharing is however not deterministic.
 *    In future lessons we shall go about how to use a round robin and the pre-emptive scheduler
 */ 

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

void green_led_task(void* pvParameter){

  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(400));

  }
}

void orange_led_task(void* pvParameter){

  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(250));

  }
}

void red_led_task(void* pvParameter){

  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(100));

  }
}

void blue_led_task(void* pvParameter){

  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));

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

  xTaskCreate(blue_led_task,"Blue led cotroller",100,NULL,1,NULL);
  xTaskCreate(green_led_task,"Green led cotroller",100,NULL,2,NULL);
  xTaskCreate(orange_led_task,"Orange led cotroller",100,NULL,1,NULL);
  xTaskCreate(red_led_task,"Red led cotroller",100,NULL,1,NULL);
  
  
  vTaskStartScheduler();

  while(1){

  }
}
