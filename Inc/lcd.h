/*
 * lcd.h
 *
 *  Created on: Apr 11, 2018
 *      Author: jared
 */

#ifndef LCD_H_
#define LCD_H_

#include "stm32f0xx_hal.h"

void lcd_init();
void clear_screen();
void lcd_write(char *string);
void position_cursor(int row, int column);
void shift(uint8_t dir); // dir is defined in smart_dorm.c

#endif /* LCD_H_ */


