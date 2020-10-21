#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/******************************************************************
 * In this exercise we practice how to create tasks and change
 * task priorities at runtime
 * ****************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

//Here we create task Handles
TaskHandle_t green_led_handle;
TaskHandle_t orange_led_handle;
TaskHandle_t red_led_handle;
TaskHandle_t blue_led_handle;


void vGreen_led_controller(void* pvParameter){
  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    green_led.toggle();
    //change back green led controller priority to 1
    vTaskPrioritySet(NULL,1);
  }
}

void vOrange_led_controller(void* pvParameter){
  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    orange_led.toggle();
  }
}

void vRed_led_controller(void* pvParameter){
  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    red_led.toggle();
    //Change green Led controller priority
    vTaskPrioritySet(green_led_handle,2);
  }
}

void vBlue_led_controller(void* pvParameter){
  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
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
  //Initialize pin Settings
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  xTaskCreate(vGreen_led_controller,
              "Green LED Controller",
              100,
              NULL,
              1,
              &green_led_handle);
  xTaskCreate(vOrange_led_controller,
              "Orange LED controller",
              100,
              NULL,
              1,
              &orange_led_handle);
  xTaskCreate(vRed_led_controller,
              "Red LED Controller",
              100,
              NULL,
              1,
              &red_led_handle);
  xTaskCreate(vBlue_led_controller,
              "Blue LED Controller",
              100,
              NULL,
              1,
              &blue_led_handle);

  vTaskStartScheduler(); //The freeRTOS scheduler takes over from here

  while(1){

  }
}