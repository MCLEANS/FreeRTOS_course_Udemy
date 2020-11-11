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

TaskHandle_t green_task_handle;
TaskHandle_t orange_task_handle;
TaskHandle_t red_task_handle;
TaskHandle_t blue_task_handle;

void green_led_task(void* pvParameter){

  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void orange_led_task(void* pvParameter){

  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void blue_led_task(void* pvParameter){

  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void red_led_task(void* pvParameter){

  TickType_t last_wake_time = xTaskGetTickCount();
  TickType_t delay_period = pdMS_TO_TICKS(450);
  while(1){
    red_led.toggle();
    vTaskDelayUntil(&last_wake_time,delay_period);
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

  xTaskCreate(green_led_task,
              "Green LED Task Controller",
              100,
              NULL,
              1,
              &green_task_handle);

  xTaskCreate(orange_led_task,
              "Orange LED Task Controller",
              100,
              NULL,
              1,
              &orange_task_handle);

  xTaskCreate(blue_led_task,
              "Blue LED Task Controller",
              100,
              NULL,
              1,
              &blue_task_handle);

  xTaskCreate(red_led_task,
              "Red LED Task Controller",
              100,
              NULL,
              1,
              &red_task_handle);

  vTaskStartScheduler();

  while(1){

  }
}


