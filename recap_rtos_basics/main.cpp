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

/* System task handles */
TaskHandle_t red_led_task;
TaskHandle_t blue_led_task;
TaskHandle_t green_led_task;
TaskHandle_t orange_led_task;

/**
 * System Tasks
 */
void red_task(void* pvParam){
  while(1){
    /* Toggle LED */
    red_led.toggle();
    /* create task delay */
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void blue_task(void* pvParam){
  while(1){
    /* Toggle LED */
    blue_led.toggle();
    /* create task delay */
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}

void orange_task(void* pvParam){
  while(1){
    /* Toggle LED */
    orange_led.toggle();
    /* create task delay */
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void green_task(void* pvParam){
  while(1){
    /* Toggle LED */
    green_led.toggle();
    /* create task delay */
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

int main(void) {
  /* initialize the system clock */
  system_clock.initialize();

  /* Configure pins for output */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);

  /* Configure pin output settings */
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::HIGH);

  /* Create system tasks */
  xTaskCreate(red_task,
              "Red LED controller",
              100,
              NULL,
              1,
              &red_led_task);
  xTaskCreate(blue_task,
              "Blue LED controller",
              100,
              NULL,
              1,
              &blue_led_task);
  xTaskCreate(orange_task,
              "Orange LED controller",
              100,
              NULL,
              1,
              &orange_led_task);
  xTaskCreate(green_task,
              "Green LED controller",
              100,
              NULL,
              1,
              &green_led_task); 
               
  /* start the system scheduler */
  vTaskStartScheduler();

 while(1){

  }
}
