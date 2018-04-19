/*
 * real_time.c
 *
 *  Created on: Apr 18, 2018
 *      Author: jared
 */

#include "real_time.h"
#include "stm32f0xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void set_date(uint8_t day, uint8_t month, int year)
{
    RTC_DateTypeDef date_struct;

    date_struct.Month = month;
    date_struct.Year = year % 100;
    date_struct.Date = day;

    HAL_RTC_SetDate(&hrtc, &date_struct, RTC_FORMAT_BCD);
}

void set_time(uint8_t hours, uint8_t minutes)
{
    RTC_TimeTypeDef time_struct;

    time_struct.Hours = hours;
    time_struct.Minutes = minutes;

    HAL_RTC_SetTime(&hrtc, &time_struct, RTC_FORMAT_BCD);
}

char *get_time(char *time_str)
{
    RTC_TimeTypeDef time_struct;
    RTC_DateTypeDef date_struct;

    HAL_RTC_GetTime(&hrtc, &time_struct, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &date_struct, RTC_FORMAT_BCD);

    uint8_t hours = time_struct.Hours;
    uint8_t minutes = time_struct.Minutes;

    uint8_t day = date_struct.Date;
    uint8_t month = date_struct.Month;
    uint8_t year = date_struct.Year;

    char hours_tens = hours / 10 + '0';
    char hours_ones = hours % 10 + '0';
    char minutes_tens = minutes / 10 + '0';
    char minutes_ones = minutes % 10 +'0';

    char day_tens = day / 10 +'0';
    char day_ones = day % 10 +'0';
    char month_tens = month / 10 + '0';
    char month_ones = month % 10 + '0';
    //year can only be between 0-99
    char year_tens = year / 10 + '0';
    char year_ones = year % 10 + '0';

    //char time_str[16] = {0}; //format XX:XX XX/XX/XXXX
    time_str[0] = hours_tens;
    time_str[1] = hours_ones;
    time_str[2] = ':';
    time_str[3] = minutes_tens;
    time_str[4] = minutes_ones;
    time_str[5] = ' ';
    time_str[6] = month_tens;
    time_str[7] = month_ones;
    time_str[8] = '/';
    time_str[9] = day_tens;
    time_str[10] = day_ones;
    time_str[11] = '/';
    time_str[12] = '2';
    time_str[13] = '0';
    time_str[14] = year_tens;
    time_str[15] = year_ones;

    return time_str;
}
