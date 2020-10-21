#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"

/******************************************************************************
 * 1. Suspending a task
 * ***************************************************************************/

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

TaskHandle_t green_led_handle;
TaskHandle_t orange_led_handle;
TaskHandle_t red_led_handle;
TaskHandle_t blue_led_handle;

uint32_t suspend_monitor = 0;
uint32_t resume_monitor = 0;
bool is_suspended = false;

void green_led_task(void* pvParameter){
  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000));
    green_led.toggle();
  }
}

void orange_led_task(void* pvParameter){
  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000));
    orange_led.toggle();
  }
}

void blue_led_task(void* pvParameter){
  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000));
    blue_led.toggle();
    if(is_suspended){
      resume_monitor++;
      if(resume_monitor >= 50){
          vTaskResume(red_led_handle);
          is_suspended = false;
          resume_monitor = 0;
          suspend_monitor = 0;
      }
    }
  }
}

void red_led_task(void* pvParameter){
  while(1){
    vTaskDelay(pdMS_TO_TICKS(1000));
    red_led.toggle();
   suspend_monitor++;
   if(!is_suspended){
      if(suspend_monitor >= 50){
        is_suspended = true;
        suspend_monitor = 0;
        vTaskSuspend(NULL); //Suspend the green LED controller task
      }
   }
   
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

  xTaskCreate(green_led_task,"Green led cotroller",100,NULL,1,&green_led_handle);
  xTaskCreate(orange_led_task,"Green led cotroller",100,NULL,1,&orange_led_handle);
  xTaskCreate(red_led_task,"Green led cotroller",100,NULL,1,&red_led_handle);
  xTaskCreate(blue_led_task,"Green led cotroller",100,NULL,1,&blue_led_handle);
  
  vTaskStartScheduler();

  while(1){

  }
}