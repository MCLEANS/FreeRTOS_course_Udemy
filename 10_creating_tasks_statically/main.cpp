#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/**
 * 1. Add (#define configSUPPORT_STATIC_ALLOCATION 1) into the FreeRTOSConfig.h file
 * 2. Create vApplicationIdleTaskMemory callback
 * 3. Create vApplicationTimerTaskMemory callback
 */ 

custom_libraries::clock_config system_clock;

custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

//Blue task memory allocation
#define blue_led_task_STACK_SIZE 100
StaticTask_t blue_led_task_buffer;
StackType_t blue_led_task_STACK[blue_led_task_STACK_SIZE]; //StackType_t is defined as uint32_t 

//Red task Memory allocation
#define red_led_task_STACK_SIZE 100
StaticTask_t red_led_task_buffer;
StackType_t red_led_task_STACK[red_led_task_STACK_SIZE];

#define orange_led_task_STACK_SIZE 100
StaticTask_t orange_led_task_buffer;
StackType_t orange_led_task_STACK[orange_led_task_STACK_SIZE];

#define green_led_task_STACK_SIZE 100
StaticTask_t green_led_task_buffer;
StackType_t green_led_task_STACK[green_led_task_STACK_SIZE];


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
 * Red LED Task
 */
void red_led_task(void* pvParameter){

  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/**
 * Orange LED Task
 */
void orange_led_task(void* pvParameter){

  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

/**
 * Green LED Task
 */
void green_led_task(void* pvParameter){

  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
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

  xTaskCreateStatic(red_led_task,
                    "Red Led Controller Task",
                    red_led_task_STACK_SIZE,
                    NULL,
                    1,
                    red_led_task_STACK,
                    &red_led_task_buffer);

  xTaskCreateStatic(orange_led_task,
                    "Orange Led Controller Task",
                    orange_led_task_STACK_SIZE,
                    NULL,
                    1,
                    orange_led_task_STACK,
                    &orange_led_task_buffer);

  xTaskCreateStatic(green_led_task,
                    "Green Led Controller task",
                    green_led_task_STACK_SIZE,
                    NULL,
                    1,
                    green_led_task_STACK,
                    &green_led_task_buffer);

  vTaskStartScheduler();
  
  while(1){

  }
}


/**
 * Timer Task Memory Callback
 */ 
extern "C" void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize){
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * Idle Task Memory Callback
 */ 
extern "C" void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize){
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}