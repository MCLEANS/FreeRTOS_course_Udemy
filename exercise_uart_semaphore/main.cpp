#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include <semphr.h>

#include "USART.h"
#include "GPIO.h"

/**
* 1.Usage of semaphores for microcontroller resource sharing.
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

/**
 * Semaphore handles
 */
SemaphoreHandle_t uart_semaphore;

void send_greeting(void* pvParameter){
  char greeting[] = "Hello World";
  while(1){
    if(xSemaphoreTake(uart_semaphore,portMAX_DELAY) != pdFALSE){
      serial.println(greeting);
      orange_led.toggle();
      xSemaphoreGive(uart_semaphore);
    }      
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void send_notification(void* pvParameter){
  char notification[] = "Sending notification";
  while(1){
    if(xSemaphoreTake(uart_semaphore,portMAX_DELAY) != pdFALSE){
      serial.println(notification);
      green_led.toggle();
      xSemaphoreGive(uart_semaphore);
    }
    vTaskDelay(pdMS_TO_TICKS(50));
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
   * Create USART3 semaphore
   */
  uart_semaphore = xSemaphoreCreateBinary();

  /**
   * Remember to give semaphore before attempting to take
   */
  xSemaphoreGive(uart_semaphore);

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