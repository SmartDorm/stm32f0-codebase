/*
 * idle.c
 *
 * This file contains the code that describes the idle state of the smart dorm.
 * Main features of this include passive information display and time display.
 * Listens for state change using UART_Receive_IT.
 *
 */
#include "main.h"
#include "stm32f0xx_hal.h"
#include "smart_dorm.h"
#include <malloc.h>
// defines
#define CHANGE 5

typedef enum {weather, twitter, calendar, final} DisplayInfo;

extern UART_HandleTypeDef huart1;

// globals
bool received = false;
uint16_t time = 0; // Count IRQs, wait for certain time to change information
uint16_t elapsed_seconds = 0; // increment every second. when elapsed_seconds % CHANGE == 0, update display.
uint8_t * buf[200];
int pos = 0;
bool change_state = false;

// TIMEKEEPING FOR FETCHES WILL BE DONE BY PI ITSELF, NOT THIS PROGRAM. DISREGARD
// When elapsed_seconds == 600, re-fetch weather.
// News headlines: fetch once per day, cycle through list of retrieved headlines

/*
 * 1. Call 'listen' to wait for a phone connection
 * 2. Every X seconds (TBD) fetch new information to display to LCD
 * 3. Increment the next state
 */
State idle_s() {
    char * datetime = malloc(sizeof(char) * 20);
    if(datetime == NULL) {
        return err;
    }
    listen(); // wait for a phone command
    DisplayInfo next = calendar;
    char * high;
    char * low;
    char * current;
    char * condition;
    while(true) {
        if(received) {
            received = false;
            return connect;
        }
        if(time == CHANGE) {
            if(next == weather) {
                buf = fetch("WTHR");
                sscanf(buf, "%s,%s,%s,%s", high, low, curent, condition);
            } else {
                // RTC calendar data
                datetime = get_time();
                lcd_write(datetime);
            }

            // set next display state
            next++;
            if(next == final) {
                next = weather;
            }
        }
    }
}

void fetch(char * command) {
    // SPI To our internet interface
//    char * buf = malloc(sizeof(*buf) * 200);
//    if(buf == NULL) {
//        return NULL;
//    }

    HAL_UART_Transmit(&huart1, "WTHR", 4, 1);
    HAL_UART_Receive_IT(&huart1, buf, 1);
    while(!received);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart) {
    if(huart->Instance == huart1) {
        if(buf[pos++] == "\0") {
            received = true;
        } else {
            HAL_UART_Receive_IT(&huart1, buf, 1);
        }
    }
}
