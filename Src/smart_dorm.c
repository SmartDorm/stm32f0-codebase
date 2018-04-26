/*
 * smart_dorm.c
 *
 *  Created on: Apr 14, 2018
 *      Author: chili
 */

#include "main.h"
#include "stm32f0xx_hal.h"
#include "smart_dorm.h"
#include "lcd.h"
#include "real_time.h"
#include "pi_client.h"
#include <string.h>
#include <stdlib.h>

// Import all peripheral structures
extern DAC_HandleTypeDef hdac1;
extern RTC_HandleTypeDef hrtc;
extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

// Fan speed defs
#define FAN_HI  100
#define FAN_LO  80
#define FAN_OFF 0
#define FAN_STATE_PERIOD 10

#define UPDATE_PERIOD 20

#define LEFT 0x55
#define RIGHT 0x56

// LED state macros
#define __GREEN_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define __YELLOW_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET)
#define __RED_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define __GREEN_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)
#define __YELLOW_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET)
#define __RED_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET)

// state variables
int IRQ_Count = 0;
int secs = 0;
int last_fan_state = FAN_OFF;
int fan_change_time = 0;
int last_update = 0;
bool change_fan = false;
bool update = false;

State state = idle;
Info current_info = {.high = 10, .low = 1, .current = 5, .headline = "Florida man kills dog"};
DisplayInfo displayed = calendar;

// USART Data buffers
uint8_t command_UPDATE[4] = {'U', 'P', 'D', 'T'};
uint8_t command_TIME[4] = {'T', 'I', 'M', 'E'};
char buf[80];
char single[1];
int posb = 0; // USART1
int posc = 0; // USART2
bool received = false;
bool command = false;
char control_word[15];
bool transmit = false;
bool change_display = false;

void app_main() {

    init();

    while(true) {
            if(state == music) {

            }
            if(state == err) {
                init();
            }

            if(update) {
                update = false;
            }

            if(received) {
                received = false;
                __YELLOW_OFF();
                clear_screen();
                HAL_Delay(100);
                lcd_write(buf);
                HAL_Delay(100);
                posb = 0;
            }
            if(secs == 60) {
                secs = 0;
                // clock_display();
            }
            if(change_display) {
                displayNext();
                change_display = false;
            }

//            if(change_fan) {
//                change_fan = false;
//                switch(last_fan_state) {
//                    case FAN_OFF:   clear_screen();
//                                    HAL_Delay(100);
//                                    lcd_write("LOW");
//                                    HAL_Delay(100);
//                                    last_fan_state = FAN_LO;
//                    break;
//
//                    case FAN_LO:    clear_screen();
//                                    HAL_Delay(100);
//                                    lcd_write("HIGH");
//                                    HAL_Delay(100);
//                                    last_fan_state = FAN_HI;
//                    break;
//
//                    default:        clear_screen();
//                                    HAL_Delay(100);
//                                    lcd_write("OFF");
//                                    HAL_Delay(100);
//                                    last_fan_state = FAN_OFF;
//                    break;
//                }
//                __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, last_fan_state);
//            }
        }
}

void displayNext() {
    // IDEA: Make more display states (Current, high, low, etc.)
    // UPDATE this state every state display change
    // __ F CONDITION
    // HI: __ LO: __
    if(displayed == calendar) {
        // display 2-line weather
        displayed = weather;
        write_weather();

    } else if(displayed == weather) {
        displayed = news;
        write_headlines();
    } else {
        displayed = calendar;
        write_time();
    }
}

void write_headlines() {
    clear_screen();
    HAL_Delay(100);
    lcd_write(current_info.headline);
    HAL_Delay(2000);
    for(int i = 0; i < 25; i++) {
        shift(LEFT);
        HAL_Delay(1000);
    }
}

void write_weather() {
    char weather1[17];
    char weather2[17];
    sprintf(weather1, "%d F  %s", current_info.current, current_info.condition);
    sprintf(weather2, "HI: %d, LO: %d", current_info.high, current_info.low);

    clear_screen();
    HAL_Delay(100);
    position_cursor(1, 1);
    HAL_Delay(100);
    lcd_write(weather1);
    HAL_Delay(100);
    position_cursor(2, 1);
    HAL_Delay(100);
    lcd_write(weather2);
    HAL_Delay(100);
}

void fetch() {
    // RETURN FORMAT:
    //   CURRENT,HIGH,LOW,HEADLINE
    __YELLOW_ON();
    __RED_ON();
    HAL_UART_Transmit_IT(&huart1, command_UPDATE, 4);
    while(!transmit);
    transmit = false;
    HAL_Delay(100);
    HAL_UART_Receive_IT(&huart1, single, 1);
    while(!received);
    strtok(buf, ",");
    char * token;
    int temp;
    // Update weather
    token = strtok(NULL, ",");
    temp = atoi(token);
    current_info.current = temp;
    token = strtok(NULL, ",");
    current_info.high = temp;
    token = strtok(NULL, ",");
    current_info.low = temp;
    token = strtok(NULL, ",");

    //headline
    token = strtok(NULL, ",");
    strcpy(current_info.headline, token);
    __RED_OFF();
}

void error_handler() {
    __GREEN_OFF();
    __RED_ON();
    init();
    __RED_OFF();
    app_main();
}

void init() {
    // Add all starts of peripherals here (i.e. HAL_TIM2_Base_Start_IT)
    // init_time();
    __YELLOW_ON();
    lcd_init();
    clear_screen();
    HAL_Delay(100);
    HAL_TIM_Base_Start_IT(&htim2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, FAN_OFF);

    // request time
//    HAL_UART_Transmit(&huart1, (uint8_t *) command_TIME, 4, 10);
//    HAL_UART_Receive_IT(&huart1, (uint8_t *) single, 1);
//    while(!received);
//    received = false;

    // set clock
    strtok(buf, ",");
    char * token;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hours;
    uint8_t mins;
    // YYYY/MM/DD
    token = strtok(NULL, ",");
    year = atoi(token); // YEAR
    token = strtok(NULL, ",");
    month = atoi(token); // MONTH
    token = strtok(NULL, ",");
    day = atoi(token); // DAY


    //HH:MM
    token = strtok(NULL, ",");
    hours = atoi(token); //HOUR
    token = strtok(NULL, ",");
    mins = atoi(token); //MINUTE

    set_time(hours, mins);
    set_date(0, day, month, year);

    //fetch();

    write_time();

    HAL_Delay(100);
    __YELLOW_OFF();
    __GREEN_ON();
}

void write_time() {
    clear_screen();
    HAL_Delay(100);
    char buf3[17];
    lcd_write(get_time(buf3));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) {
    if(htim->Instance == TIM2) {
        IRQ_Count++;
        if(IRQ_Count == 1000) {
            IRQ_Count = 0;
            secs++;
        }
        if(secs > 60000) {
            secs = 0;
            fan_change_time = 0;
        }
        if(secs - fan_change_time == FAN_STATE_PERIOD) {
            change_fan = true;
            fan_change_time = secs;
        }
        if(secs % 15 == 0 && IRQ_Count == 0) {
            change_display = true;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
    // FIX PYTHON CLIENT TO INCLUDE NULL TERMINATOR
    if(huart->Instance == USART1) {
        __YELLOW_ON();
        buf[posb++] = single[0];
        if(single[0] != '\n' && single[0] != '\0') {
            HAL_UART_Receive_IT(&huart1, (uint8_t *) single, 1);
        } else {
        //buf[6] = "\0";
            received = true;
        }
    }

    if(huart->Instance == USART2) {
        __YELLOW_ON();
        control_word[posc++] = single[0];
        if(single[0] == '\n' || single[0] == '\0') {
            command = true;
        }
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart) {
    if(huart->Instance == USART1) {
        // HAL_UART_Receive_IT(&huart1, (uint8_t *) single, 1);
        transmit = true;
    }
}
