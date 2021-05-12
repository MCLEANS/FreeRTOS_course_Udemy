#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include <timers.h>
#include "GPIO.h"

#define BLUE_TIMER_ID 234
#define RED_TIMER_ID 123
#define ORANGE_TIMER_ID 675
#define GREEN_TIMER_ID 345

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Task handles
 */
TaskHandle_t red_led_task;
TaskHandle_t blue_led_task;
TaskHandle_t green_led_task;
TaskHandle_t orange_led_task;

/**
 * Timer handles
 */
TimerHandle_t red_timer;
TimerHandle_t blue_timer;
TimerHandle_t orange_timer;
TimerHandle_t green_timer;

/**
 * System tasks
 */
void red_blink(void* pvParam){
  while(1){

  }
}

void blue_blink(void* pvParam){
  while(1){

  }
}

void orange_blink(void* pvParam){
  while(1){

  }
}

void green_blink(void* pvParam){
  while(1){

  }
}

void red_led_callback(TimerHandle_t xTimer){
  blue_led.toggle();
}

void blue_led_callback(TimerHandle_t xTimer){
  red_led.toggle();
}

void green_led_callback(TimerHandle_t xTimer){
  green_led.toggle();
}

void orange_led_callback(TimerHandle_t xTimer){
  orange_led.toggle();
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
              "Blue Led controller",
              100,
              NULL,
              1,
              &blue_led_task);
  xTaskCreate(green_blink,
              "Green Led controller",
              100,
              NULL,
              1,
              &green_led_task);
  xTaskCreate(orange_blink,
              "Orange Led controller",
              100,
              NULL,
              1,
              &orange_led_task);

  red_timer = xTimerCreate("Red Led Timer",
                          pdMS_TO_TICKS(1000),
                          true,
                          (void*)RED_TIMER_ID,
                          red_led_callback);
  if(red_timer != NULL){
    xTimerStart(red_timer,0);
  }

  blue_timer = xTimerCreate("Blue Led Timer",
                            pdMS_TO_TICKS(200),
                            true,
                            (void*)BLUE_TIMER_ID,
                            blue_led_callback);
  if(blue_timer != NULL){
    xTimerStart(blue_timer,0);
  }

  orange_timer = xTimerCreate("Orange Led Timer",
                          pdMS_TO_TICKS(500),
                          true,
                          (void*)ORANGE_TIMER_ID,
                          orange_led_callback);
  if(orange_timer != NULL){
    xTimerStart(orange_timer,0);
  }

  green_timer = xTimerCreate("Green Led Timer",
                              pdMS_TO_TICKS(100),
                              true,
                              (void*)GREEN_TIMER_ID,
                              green_led_callback);
  if(green_timer != NULL){
    xTimerStart(green_timer,0);
  }                        

  vTaskStartScheduler();

  while(1){

  }
}
