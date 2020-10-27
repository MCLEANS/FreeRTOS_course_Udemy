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

#define blue_led_task_STACK_SIZE 100
StaticTask_t blue_led_task_buffer;
StackType_t blue_led_task_STACK[100]; //StackType_t is defined as uint32_t 

/**
 * Blue LED Task
 */

void blue_led_task(void* pvParameter){

  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

/**
 * Idle Task Memory Callback
 */ 
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize){
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


int main(void) {

  system_clock.initialize();
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  xTaskCreateStatic(blue_led_task,
                    "Blue Led Controller task",
                    blue_led_task_STACK_SIZE,
                    NULL,
                    1,
                    blue_led_task_STACK,
                    &blue_led_task_buffer);
  
  while(1){

  }
}