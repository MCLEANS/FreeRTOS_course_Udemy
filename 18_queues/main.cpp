#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "USART.h"

/**
 * 1. QUEUEs are used for inter-task communication.
 * 2. Data is loaded onto a FIFO buffer by one task and read out by another task
 */

/**
 * Define UART pins and baudrate
 */
#define SERIAL_RX 11
#define SERIAL_TX 10
#define BAUDRATE 9600
/**
 * Initialization of system clock object
 */
custom_libraries::clock_config system_clock;

/**
 * Initialization of UART object
 */
custom_libraries::USART serial(USART3,GPIOC,custom_libraries::_DMA1,DMA1_Stream1,SERIAL_RX,SERIAL_TX,BAUDRATE);


/**
 * Initializing GPIO obects
 */
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);


int main(void) {
  /**
   * Initialze system clock
   */
  system_clock.initialize();

  /**
   * Initialize UART port
   */
  serial.initialize();

  /**
   * Configure GPIO pins to output
   */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::MEDUIM);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::MEDUIM);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::MEDUIM);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::MEDUIM);

  while(1){

  }
}
