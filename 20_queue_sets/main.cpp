#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "LIS3DH.h"
#include "EXTI.h"

#define INTERRUPT_PORT GPIOA
#define INTERRUPT_PIN 4

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/* Create interrupt  edge type */
custom_libraries::edge response_edge = custom_libraries::RISING;

/* Create interrupt object */
custom_libraries::_EXTI halt_button(INTERRUPT_PORT,INTERRUPT_PIN,response_edge);

/* LIS3DH pin definition */
#define SCK_PIN 5
#define MOSI_PIN 7
#define MISO_PIN 6

#define CS_PORT GPIOE
#define CS_PIN 3

/* Create accelerometer object */
custom_libraries::LIS3DH accelerometer(SPI1,
                                      GPIOA,
                                      SCK_PIN,
                                      MOSI_PIN,
                                      MISO_PIN,
                                      CS_PORT,
                                      CS_PIN);

TaskHandle_t sensor_read_handle;

/* External Interrupts ISR */
void EXTI4_IRQhandler(void){
  /* verify interrupt, reset flag and give semaphore */

}



int main(void) {
  
  system_clock.initialize();

  /* Initialize interrupt pin */
  halt_button.initialize();

  /* Set interrupt vector and enable */
  NVIC_SetPriority(EXTI4_IRQn, 0x05);
  NVIC_EnableIRQ(EXTI4_IRQn);

  /* Initialize accelerometer */
  accelerometer.initialize();

  //configure LEDs as output
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  xTaskCreate(sensor_read,
              )
  
  vTaskStartScheduler();

  while(1){

  }
}
