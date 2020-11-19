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
 * Task handles for the various tasks
 */ 
TaskHandle_t green_handle;
TaskHandle_t orange_handle;
TaskHandle_t red_handle;
TaskHandle_t blue_handle;

void green_led_task(void* pvParameter){
  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void orange_led_task(void* pvParameter){
  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));
    vTaskPrioritySet(red_handle,3);
  }
}

void red_led_task(void* pvParameter){
  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void blue_led_task(void* pvParameter){
  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(250));
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
              "green LED Task Controller",
              100,
              NULL,
              1,
              &green_handle);

  xTaskCreate(orange_led_task,
              "orange LED Task Controller",
              100,
              NULL,
              1,
              &orange_handle);

  xTaskCreate(red_led_task,
            "red LED Task Controller",
            100,
            NULL,
            1,
            &red_handle);

  xTaskCreate(blue_led_task,
              "blue LED Task Controller",
              100,
              NULL,
              1,
              &blue_handle);
              
  vTaskStartScheduler();
              

  while(1){

  }
}
