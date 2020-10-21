#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/*********************************************************************************
 * 1. blocking a task with vTaskDelay()
 * *******************************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

void green_led_task(void* pvParameter){

  while(1){
    vTaskDelay(pdMS_TO_TICKS(50));
    green_led.toggle();

  }
}

void orange_led_task(void* pvParameter){

  while(1){
    vTaskDelay(pdMS_TO_TICKS(50));
    orange_led.toggle();
  }
}

void red_led_task(void* pvParameter){

  while(1){
    vTaskDelay(pdMS_TO_TICKS(50));
    red_led.toggle();
  }
}

void blue_led_task(void* pvParameter){

  while(1){
    vTaskDelay(pdMS_TO_TICKS(50));
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

  xTaskCreate(green_led_task,"Green led cotroller",100,NULL,1,NULL);
  xTaskCreate(orange_led_task,"Green led cotroller",100,NULL,2,NULL);
  xTaskCreate(red_led_task,"Green led cotroller",100,NULL,3,NULL);
  xTaskCreate(blue_led_task,"Green led cotroller",100,NULL,4,NULL);
  
  vTaskStartScheduler();

  while(1){

  }
}