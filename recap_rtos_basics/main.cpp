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


 
  while(1){

  }
}
