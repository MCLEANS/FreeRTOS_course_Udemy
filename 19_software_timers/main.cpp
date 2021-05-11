#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include <timers.h>
#include "GPIO.h"

#define RED_LED_TIMER_ID 144
#define BLUE_LED_TIMER_ID 367

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/* Task handles */
TaskHandle_t red_led_task;
TaskHandle_t blue_led_task;

/* Timer handles */
TimerHandle_t red_timer;
TimerHandle_t blue_timer;

/* System tasks */
void red_blink(void* pvParam){
  while(1){

  }
}

void blue_blink(void* pvParam){
  while(1){

  }
}

void red_timer_callback(TimerHandle_t xTimer){
  red_led.toggle();
}

void blue_timer_callback(TimerHandle_t xTimer){
  blue_led.toggle();
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

  /* Create system tasks */
  xTaskCreate(red_blink,
              "Red Led controller",
              100,
              NULL,
              1,
              &red_led_task);
  xTaskCreate(blue_blink,
              "Blue Led Controller",
              100,
              NULL,
              1,
              &blue_led_task);

  /* Create system timers, they will only start running once the scheduler starts */
  red_timer = xTimerCreate("Red Led timer",
                          pdMS_TO_TICKS(100),
                          true,
                          (void*)RED_LED_TIMER_ID,
                          red_timer_callback);
  if(red_timer != NULL){
    xTimerStart(red_timer,0);
  }

  blue_timer = xTimerCreate("Blue Led Timer",
                            pdMS_TO_TICKS(500),
                            true,
                            (void*)BLUE_LED_TIMER_ID,
                            blue_timer_callback);
  if(blue_timer != NULL){
    xTimerStart(blue_timer,0);
  }
  
  vTaskStartScheduler();

  while(1){

  }
}
