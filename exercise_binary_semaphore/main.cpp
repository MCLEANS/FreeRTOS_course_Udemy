#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include <semphr.h>

#include "GPIO.h"

/**
 * Binary semaphores are suitably used for synchronisation between task and task 
 * or between an Interrupt Service Routine and a task.
 */

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Initialize button object.
 */
custom_libraries::_GPIO user_button(GPIOA,0);

/**
 * Task handles
 */
TaskHandle_t button_press_task;
TaskHandle_t blinky_task;


/**
 * Semaphore handles
 */
SemaphoreHandle_t button_semaphore;

/**
 * Project tasks
 */
void blinky(void* pvParameter){
  while(1){

  }
}

void button_press(void* pvParameter){
  while(1){

  }
}

int main(void) {
  
  system_clock.initialize();
  /**
   * Configure LEDs as output
   */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  /**
   * Cofigure user button as input
   */
  user_button.pin_mode(custom_libraries::INPUT);
  user_button.input_state(custom_libraries::PULL_UP);

  /**
   * Create object tasks
   */
  xTaskCreate(blinky,
              "Led blinking task",
              100,
              NULL,
              1,
              &blinky_task);

  xTaskCreate(button_press,
              "Button press task",
              100,
              NULL,
              1,
              &button_press_task);

  while(1){

  }
}
