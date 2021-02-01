#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
#include "LIS3DH.h"
#include "NOKIA_5110.h"

/**
 * 1. Get accelerometer values from one task and push the values to LCD display task
 * via a queue.
 * 2. Use a semaphore to toggle an LED each time a valid accelerometer value is read.
 */
custom_libraries::clock_config system_clock;

/**
 * LED object initialization
 */
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Accelerometer object Initialization
 */
#define SCK_PIN 5
#define MOSI_PIN 7
#define MISO_PIN 6

#define CS_PORT GPIOE
#define CS_PIN 3

custom_libraries::LIS3DH motion_sensor(SPI1,
                                        GPIOA,
                                        SCK_PIN,
                                        MOSI_PIN,
                                        MISO_PIN,
                                        CS_PORT,
                                        CS_PIN);

/**
 * Initialize NOKIA 5110 LCD
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
 * System task handles
 */
TaskHandle_t red_indicator_task;
TaskHandle_t blue_inidator_task;
TaskHandle_t orange_indicator_task;
TaskHandle_t green_indicator_task;
TaskHandle_t accelerometer_task;
TaskHandle_t display_task;

/**
 * System task declarations
 */
void red_indicator(void* pvParameter){
  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(100));

  }
}

void blue_indicator(void* pvParameter){
  while(1){
    blue_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));

  }
}

void orange_indicator(void* pvParameter){
  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void green_indicator(void* pvParameter){
  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(150));
  }
}

void accelerometer_handler(void* pvParameter){
   /**
   * Initialize the motion sensor
   */
  bool Accel_is_initialized = motion_sensor.initialize();
  /**
   * Check whether the Motion sensor was properly Initialized
   */
  if(!Accel_is_initialized){
    /**
     * Perform error handling here
     */
  }
  while(1){
    /**
     * Write tasks to handle obtaining data from the Accelerometer
     */

  }
}

void display_handler(void* pvParameter){
  while(1){
    /**
     * Write task to handle the NOKIA 5110 LCD Display
     */
  }
}

int main(void) {
  /**
   * Initialization of the system clock
   */
  system_clock.initialize();
  /**
   * Configure LED as output
   */
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);
  /**
   * Configuration of LED output settings
   */
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  /**
   * Create system tasks
   */
  xTaskCreate(red_indicator,
              "Red LED Indicator",
              100,
              NULL,
              1,
              &red_indicator_task);
  xTaskCreate(blue_indicator,
              "Blue LED Indicator",
              100,
              NULL,
              1,
              &blue_inidator_task);
  xTaskCreate(green_indicator,
              "Green task indicator",
              100,
              NULL,
              1,
              &green_indicator_task);
  xTaskCreate(orange_indicator,
              "Orange task idicator",
              100,
              NULL,
              1,
              &orange_indicator_task);
  xTaskCreate(accelerometer_handler,
              "Accelerometer task",
              100,
              NULL,
              1,
              &accelerometer_task);
  xTaskCreate(display_handler,
              "LCD Display task",
              100,
              NULL,
              1,
              &display_task);

  vTaskStartScheduler();

  while(1){

  }
}
