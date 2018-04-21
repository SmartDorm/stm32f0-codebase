/*
 * pi_client.c
 *
 *  Created on: Apr 21, 2018
 *      Author: chili
 */
#include "pi_client.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

void init_time() {
    // RESTRUCTURE
    char buf[80];
    int pos = 0;
    HAL_UART_Transmit(&huart1, "TIME");
    HAL_Delay(100);
    do {
        HAL_UART_Receive(&huart1, buf, 1, 1);
        HAL_Delay(5);
    } while(buf[pos] != '\0');
    // BUF WILL CONTAIN: Year,Month,Day,Hours,Minutes,Seconds
    int years;
    int months;
    int days;
    int hours;
    int minutes;
    int seconds;
    sscanf(buf, "%d,%d,%d,%d,%d,%d", &years, &months, &days, &hours, &minutes, &seconds);

    set_time((uint8_t) hours, (uint8_t) minutes);
    set_date((uint8_t) days, (uint8_t) months, (uint8_t) years);

}

