#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "USART.h"

/**
* 1.Usage of semaphores for microcontroller resource sharing.
*/ 


/**
 * Define UART pins and baudrate
 */
#define SERIAL_RX 10
#define SERIAL_TX 9
#define BAUDRATE 9600

/**
 * Initialization of system clock object
 */
custom_libraries::clock_config system_clock;

/**
 * Initialization of UART object
 */
custom_libraries::USART serial(USART1,
                              GPIOA,
                              custom_libraries::_DMA2,
                              DMA2_Stream2,
                              SERIAL_RX,
                              SERIAL_TX,
                              BAUDRATE);


int main(void) {
  
  system_clock.initialize();
  serial.initialize();

  

  while(1){

  }
}