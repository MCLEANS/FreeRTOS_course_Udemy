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

/**
 * Task handles
 */
TaskHandle_t greetings_task;
TaskHandle_t notification_task;

void send_greeting(void* pvParameter){
  char greeting[] = "Hello World";
  while(1){
    serial.println(greeting);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void send_notification(void* pvParameter){
  char notification[] = "Sending notification";
  while(1){
    serial.println(notification);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

int main(void) {
  system_clock.initialize();
  serial.initialize();

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