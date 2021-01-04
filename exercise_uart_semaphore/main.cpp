#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "USART.h"
#include "GPIO.h"

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
/**
 * Initialize GPIO pin objects
 */
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Task handles
 */
TaskHandle_t greetings_task;
TaskHandle_t notification_task;

void send_greeting(void* pvParameter){
  char greeting[] = "Hello World";
  while(1){
    serial.println(greeting);
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void send_notification(void* pvParameter){
  char notification[] = "Sending notification";
  while(1){
    serial.println(notification);
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

int main(void) {
  system_clock.initialize();
  serial.initialize();

  /**
   * Initialize GPIO pins as output
   */
  green_led.pin_mode(custom_libraries::OUTPUT);
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.pin_mode(custom_libraries::OUTPUT);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  /**
   * Create project tasks 
   */
  xTaskCreate(send_greeting,
              "Greetings task",
              100,
              NULL,
              1,
              &greetings_task);

  xTaskCreate(send_notification,
              "Notification task",
              100,
              NULL,
              1,
              &notification_task);

  vTaskStartScheduler();

  while(1){

  }
}