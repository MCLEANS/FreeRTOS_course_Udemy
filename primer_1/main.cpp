#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "NOKIA_5110.h"
#include "LIS3DH.h"

custom_libraries::clock_config system_clock;

/**
 * Initialize LED objects
 */ 
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Initialize NOKIA LCD object
 */
#define NOKIA_RST_PORT GPIOD
#define NOKIA_RST_PIN 0
#define NOKIA_CS_PORT GPIOD
#define NOKIA_CS_PIN 1
#define NOKIA_DC_PORT GPIOD
#define NOKIA_DC_PIN 2

custom_libraries::NOKIA_5110 NOKIA(SPI2,
                                    GPIOB,
                                    13,
                                    15,
                                    0,
                                    64,
                                    false,
                                    false,
                                    false,
                                    NOKIA_CS_PORT,
                                    NOKIA_CS_PIN,
                                    NOKIA_RST_PORT,
                                    NOKIA_RST_PIN,
                                    NOKIA_DC_PORT,
                                    NOKIA_DC_PIN);

/**
 * Running task handles
 */
TaskHandle_t accelerometer_angle_task;
TaskHandle_t accelerometer_display_task;
TaskHandle_t red_led_indicator_task;
TaskHandle_t blue_led_indicator_task;
TaskHandle_t green_led_indicator_task;
TaskHandle_t orange_led_indicator_task;

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
  NOKIA.print("INIT SUCCESS",5,3);
  
  while (1){

  }
  
}

/**
 * This task uses the red LED to indicate the **** AXIS angle intensity
 */ 
void red_led_indicator(void* pvParam){
  red_led.pin_mode(custom_libraries::OUTPUT);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

/**
 * This task uses the blue LED to indicate the **** AXIS angle intensity
 */ 
void blue_led_indicator(void* pvParam){
  blue_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

/**
 * This task uses the green LED to indicate the **** AXIS angle intensity
 */ 
void green_led_indicator(void* pvParam){
  green_led.pin_mode(custom_libraries::OUTPUT);
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}

/**
 * This task uses the orange LED to indicate the **** AXIS angle intensity
 */ 
void orange_led_indicator(void* pvParam){
  orange_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));
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

  xTaskCreate(red_led_indicator,
              "Uses red LED to indicate angle",
              100,
              NULL,
              1,
              &red_led_indicator_task);
              
  xTaskCreate(blue_led_indicator,
              "Uses blue LED to indicate angle",
              100,
              NULL,
              1,
              &blue_led_indicator_task);

  xTaskCreate(orange_led_indicator,
              "Uses orange LED to indicate angle",
              100,
              NULL,
              1,
              &orange_led_indicator_task);

  xTaskCreate(green_led_indicator,
              "Uses green LED to indicate angle",
              100,
              NULL,
              1,
              &green_led_indicator_task);

   vTaskStartScheduler();


  while(1){

  }
}