#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "NOKIA_5110.h"
#include "LIS3DH.h"

custom_libraries::clock_config system_clock;

TaskHandle_t accelerometer_angle_task;
TaskHandle_t accelerometer_display_task;
TaskHandle_t led_indicator_task;

/**
 * This tasks reads the angle and status form the accelerometer
 */
void read_accelerometer_status(void* pvParam){
  while(1){

  }
}

/**
 * This task displays the accelerometer values onto the LCD
 */
void display_accelerometer_values(void* pvParam){
  while (1){

  }
  
}

/**
 * This task uses the inbuilt LEDs to indicate the orientation of the Board
 */ 
void indicate_board_orientation(void* pvParam){
  while(1){

  }
}



int main(void) {
  
  system_clock.initialize();

  xTaskCreate(read_accelerometer_status,
              "Read angle values from accelerometer",
              100,
              NULL,
              1,
              &accelerometer_angle_task);

  xTaskCreate(display_accelerometer_values,
              "Displays accelerometer values on an LCD",
              100,
              NULL,
              1,
              &accelerometer_display_task);

  xTaskCreate(indicate_board_orientation,
              "Uses blinking LEDs to indicate the board orientation",
              100,
              NULL,
              1,
              &led_indicator_task);

    vTaskStartScheduler();


  while(1){

  }
}