#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>
#include <semphr.h>

#include "GPIO.h"
#include "Timerconfiguration.h"

custom_libraries::clock_config system_clock;
custom_libraries::_GPIO green_led(GPIOD,12);
custom_libraries::_GPIO orange_led(GPIOD,13);
custom_libraries::_GPIO red_led(GPIOD,14);
custom_libraries::_GPIO blue_led(GPIOD,15);

/**
 * Initialize binary semaphore
 */
SemaphoreHandle_t delay_semaphore;
/**
 * initialize the timer object
 */
custom_libraries::Timer_configuration delay_timer(TIM3,
                                                  62000,
                                                  677);

void green_led_task(void* pvParameter){

  while(1){
    green_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(1000));

  }
}

void orange_led_task(void* pvParameter){

  while(1){
    orange_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void red_led_task(void* pvParameter){

  while(1){
    red_led.toggle();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void blue_led_task(void* pvParameter){

  while(1){
   if(xSemaphoreTake(delay_semaphore,portMAX_DELAY) == pdTRUE){
      blue_led.toggle();
    }
  }
}

extern "C" void TIM3_IRQHandler(void){
  static BaseType_t xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE;

  if(TIM3->SR & TIM_SR_UIF){
    TIM3->SR &= ~(TIM_SR_UIF);
    xSemaphoreGiveFromISR(delay_semaphore,&xHigherPriorityTaskWoken);
  
    
  }
  if(xHigherPriorityTaskWoken == pdTRUE){
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
  }
}

int main(void) {
  
  system_clock.initialize();
  delay_timer.initialize();


  //configure LEDs as output
  green_led.pin_mode(custom_libraries::OUTPUT);
  orange_led.pin_mode(custom_libraries::OUTPUT);
  red_led.pin_mode(custom_libraries::OUTPUT);
  blue_led.pin_mode(custom_libraries::OUTPUT);

  green_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  orange_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  red_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);
  blue_led.output_settings(custom_libraries::PUSH_PULL,custom_libraries::VERY_HIGH);

  xTaskCreate(green_led_task,"Green led cotroller",100,NULL,1,NULL);
  xTaskCreate(orange_led_task,"Orange led cotroller",100,NULL,1,NULL);
  xTaskCreate(red_led_task,"Red led cotroller",100,NULL,1,NULL);
  xTaskCreate(blue_led_task,"Blue led cotroller",100,NULL,1,NULL);

  delay_semaphore = xSemaphoreCreateBinary();

  NVIC_SetPriority(TIM3_IRQn,0x06);
  NVIC_EnableIRQ(TIM3_IRQn);


  vTaskStartScheduler();

  while(1){

  }
}
