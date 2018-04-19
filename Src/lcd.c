/*
 * lcd.c
 *
 *  Created on: Apr 11, 2018
 *      Author: jared
 */

#include "lcd.h"
#include "stm32f0xx_hal.h"
#include <string.h>

extern SPI_HandleTypeDef hspi1;

void clear_screen()
{
    uint8_t clear = 0xFE;
    HAL_SPI_Transmit(&hspi1, &clear, sizeof(clear), 100);
    uint8_t clear2 = 0x51;
    HAL_SPI_Transmit(&hspi1, &clear2, sizeof(clear2), 100);
}
void lcd_write(char *string)
{
    HAL_SPI_Transmit(&hspi1, string, strlen(string), 100);
}
