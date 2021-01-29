#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/****************************************************************
 * Here we use the task parameter to pass an Id to it and 
 * blink a specific led with regards to the ID.
 * *************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

const uint8_t BLUE_LED_ID = 1;
const uint8_t RED_LED_ID =  2;
const uint8_t ORANGE_LED_ID = 3; 
const uint8_t GREEN_LED_ID = 4;

const uint8_t* _blue_led = &BLUE_LED_ID;
const uint8_t* _red_led = &RED_LED_ID;
const uint8_t* _orange_led = &ORANGE_LED_ID;
const uint8_t* _green_led = &GREEN_LED_ID;


void vLed_controller_task(void* pvParameter){
  uint8_t* id = (uint8_t*) pvParameter;
  while(1){
    for(volatile int i = 0; i < 500000; i++){}
    if(*id == 1) blue_led.toggle();
    if(*id == 2) red_led.toggle();
    if(*id == 3) orange_led.toggle();
    if(*id == 4) green_led.toggle();
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

  //create tasks
  xTaskCreate(vLed_controller_task,
              "Blue led controller",
              100,
              (void *)_blue_led,
              1,
              NULL);
  xTaskCreate(vLed_controller_task,
              "Red led controller",
              100,
              (void *)_red_led,
              1,
              NULL);
  xTaskCreate(vLed_controller_task,
              "Green led controller",
              100,
              (void *)_green_led,
              1,
              NULL);
  xTaskCreate(vLed_controller_task,
              "orange led controller",
              100,
              (void *)_orange_led,
              1,
              NULL);

  vTaskStartScheduler();

  while(1){

  }
}