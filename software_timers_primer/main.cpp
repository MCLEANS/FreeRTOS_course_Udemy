#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include <timers.h>

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Timer handles
 */
TimerHandle_t red_timer_handle;
TimerHandle_t blue_timer_handle;

void toggle_timer_callback(TimerHandle_t xTimer){
  uint32_t timer_id = (uint32_t) pvTimerGetTimerID(xTimer);
  if(timer_id == 0){
    red_led.toggle();
  }
  else if(timer_id == 1){
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

  /**
   * Create software timer
   */
  red_timer_handle = xTimerCreate("Red LED timer",
                                  pdMS_TO_TICKS(500),
                                  pdTRUE,
                                  (void*)0,
                                  toggle_timer_callback);
  if(red_timer_handle != NULL){
    xTimerStart(red_timer_handle,0);
  }

  blue_timer_handle = xTimerCreate("Blue LED timer",
                                    pdMS_TO_TICKS(200),
                                    pdTRUE,
                                    (void*)1,
                                    toggle_timer_callback);
  if(blue_timer_handle != NULL){
    xTimerStart(blue_timer_handle,0);
  }
  
  vTaskStartScheduler();

  while(1){

  }
}
