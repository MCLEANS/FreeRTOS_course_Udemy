#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/*****************************************************************
 * 1. creating periodic tasks using vTaskDelayUntil()
 * ***************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

void green_led_task(void* pvParameter){

  while(1){
    for(int i = 0; i < 5000000; i++){}
    green_led.toggle();

  }
}

void orange_led_task(void* pvParameter){
  TickType_t xLast_wakeup_time = xTaskGetTickCount();;
  TickType_t period = pdMS_TO_TICKS(50);
  while(1){
    orange_led.toggle();
    vTaskDelayUntil(&xLast_wakeup_time,period);
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

  xTaskCreate(green_led_task,"Green led cotroller",100,NULL,1,NULL);
  xTaskCreate(orange_led_task,"Green led cotroller",100,NULL,1,NULL);
  xTaskCreate(red_led_task,"Green led cotroller",100,NULL,1,NULL);
  xTaskCreate(blue_led_task,"Green led cotroller",100,NULL,1,NULL);
  
  vTaskStartScheduler();

  while(1){

  }
}