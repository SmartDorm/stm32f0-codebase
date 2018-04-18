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
// defines
#define CHANGE 5

typedef enum {weather, twitter, calendar, final} DisplayInfo;

// globals
bool received = false;
uint16_t time = 0; // Count IRQs, wait for certain time to change information

/*
 * 1. Call 'listen' to wait for a phone connection
 * 2. Every X seconds (TBD) fetch new information to display to LCD
 * 3. Increment the next state
 */
State idle_s() {
    listen();
    DisplayInfo next = calendar;
    while(true) {
        if(received) {
            received = false;
            return connect;
        }
        if(time == CHANGE) {
            if(next != calendar) {
                fetch(next);
            } else {
                // RTC calendar data
            }

            // set next display state
            next++;
            if(next == final) {
                next = weather;
            }
        }
    }
}

void fetch() {
    // SPI To our internet interface
}
