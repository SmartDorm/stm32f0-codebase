/*
 * real_time.h
 *
 *  Created on: Apr 18, 2018
 *      Author: jared
 */

#ifndef REAL_TIME_H_
#define REAL_TIME_H_



#endif /* REAL_TIME_H_ */

#include "stm32f0xx_hal.h"

char *get_time(char *time_str);

void set_date(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year);

void set_time(uint8_t hours, uint8_t minutes);
