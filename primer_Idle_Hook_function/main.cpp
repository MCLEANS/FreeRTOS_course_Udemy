#include "stm32f4xx.h"
#include "clockconfig.h"
#include <FreeRTOS.h>
#include <task.h>
#include <portmacro.h>

/**
 * 1. An idle task will always have the lowest priority (priority 0)
 * 2. It will only run when there are no higher priority tasks runining (tasks such as memory cleanup can be runnned here)
 * 3. In order to use the idle hook function you have to enable it in the FreeRTOS config file.
 * 4. To use the idle hook we use the callback vApplicationIdleHook() 
 */

custom_libraries::clock_config system_clock;

void led1_task(void* pvParameter){
  while(1){
    for(volatile int i = 0; i < 2000000; i++){}
    GPIOD->ODR ^= (1<<12);
  }
}

void led2_task(void* pvParameter){
  while(1){
    GPIOD->ODR ^= (1<<13);
    vTaskDelay(pdMS_TO_TICKS(250));
  }
}

extern "C" void vApplicationIdleHook(void){

}

int main(void) {
  
  system_clock.initialize();
  
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  GPIOD->MODER |= GPIO_MODER_MODER12_0;
  GPIOD->ODR |= GPIO_ODR_ODR_12;
  GPIOD->MODER |= GPIO_MODER_MODER13_0;
  GPIOD->ODR |= GPIO_ODR_ODR_13;

  xTaskCreate(led1_task,"led 1 controller",100,NULL,1,NULL);
  vTaskStartScheduler();

  while(1){

  }
}