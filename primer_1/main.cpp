#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

#include "GPIO.h"
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
 * Initialize LIS3DH Accelerometer object
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
 * Running task handles
 */
TaskHandle_t accelerometer_angle_task;
TaskHandle_t accelerometer_display_task;
TaskHandle_t red_led_indicator_task;
TaskHandle_t blue_led_indicator_task;
TaskHandle_t green_led_indicator_task;
TaskHandle_t orange_led_indicator_task;

/**
 * Global variables
 */
bool LIS3DH_isInitialized = false;
custom_libraries::Angle_values angle_values;

/**
 * This tasks reads the angle and status form the accelerometer
 */
void read_accelerometer_status(void* pvParam){
  LIS3DH_isInitialized = motion_sensor.initialize();

  while(1){
    angle_values = motion_sensor.read_angles();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/**
 * This task uses the red LED to indicate the **** AXIS angle intensity
 */ 
void red_led_indicator(void* pvParam){
  red_led.pin_mode(custom_libraries::OUTPUT);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    if(angle_values.x_clockwise == false){
      red_led.toggle();
    }
    else{
      red_led.digital_write(0);
    }
    vTaskDelay(pdMS_TO_TICKS((1000-(angle_values.x_axis*9))));
  }
}

/**
 * This task uses the blue LED to indicate the **** AXIS angle intensity
 */ 
void blue_led_indicator(void* pvParam){
  blue_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    if(angle_values.y_clockwise == true){
      blue_led.toggle();
    }
    else{
      blue_led.digital_write(0);
    }
    vTaskDelay(pdMS_TO_TICKS((1000-(angle_values.y_axis*9))));
  }

  }

/**
 * This task uses the green LED to indicate the **** AXIS angle intensity
 */ 
void green_led_indicator(void* pvParam){
  green_led.pin_mode(custom_libraries::OUTPUT);
  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    if(angle_values.x_clockwise == true){
      green_led.toggle();
    }
    else{
      green_led.digital_write(0);
    }
    vTaskDelay(pdMS_TO_TICKS((1000-(angle_values.x_axis*9))));
  }
}

/**
 * This task uses the orange LED to indicate the **** AXIS angle intensity
 */ 
void orange_led_indicator(void* pvParam){
  orange_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  while(1){
    if(angle_values.y_clockwise == false){
      orange_led.toggle();
    }
    else{
      orange_led.digital_write(0);
    }
    vTaskDelay(pdMS_TO_TICKS((1000-(angle_values.y_axis*9))));
  }
}

int main(void) {
  
  system_clock.initialize();

  xTaskCreate(read_accelerometer_status,
              "Read angle values from accelerometer",
              100,
              NULL,
              3,
              &accelerometer_angle_task);

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