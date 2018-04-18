/*
 * smart_dorm.c
 *
 *  Created on: Apr 14, 2018
 *      Author: chili
 */

#include "main.h"
#include "stm32f0xx_hal.h"
#include "smart_dorm.h"

// Import all peripheral structures
extern DAC_HandleTypeDef hdac1;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

State state = idle;

void app_main() {
    // check for state. every function must be able to return a state.
    while(true) {
        if(state == idle) {

        } else if(state == music) {

        } else if(state == connect) {

        } else if(state == err) {
            init();
        }
    }
}

void init() {
    // Add all starts of peripherals here (i.e. HAL_TIM2_Base_Start_IT)
}
