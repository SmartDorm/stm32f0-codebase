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

void lcd_init()
{
    uint8_t command = 0xFE;
    uint8_t clear = 0x51;
    uint8_t on = 0x41;
    uint8_t home = 0x46;
    uint8_t brightness = 0x53;
    uint8_t brightness_level = 8;

    HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 100);
    HAL_SPI_Transmit(&hspi1, &on, sizeof(on), 100);

    HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 100);
    HAL_SPI_Transmit(&hspi1, &clear, sizeof(clear), 100);

    HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 100);
    HAL_SPI_Transmit(&hspi1, &home, sizeof(home), 100);

    HAL_SPI_Transmit(&hspi1, &command, sizeof(command), 100);
    HAL_SPI_Transmit(&hspi1, &brightness, sizeof(brightness), 100);
    HAL_SPI_Transmit(&hspi1, &brightness_level, sizeof(brightness_level), 100);
}
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
    HAL_Delay(100);
}

void position_cursor(int row, int column)
{
    uint8_t one = 0xFE;
    uint8_t two = 0x4B;
    HAL_SPI_Transmit(&hspi1, &one, sizeof(one), 100);
    HAL_SPI_Transmit(&hspi1, &two, sizeof(two), 100);

    uint8_t prefix = 0xFE;
    HAL_SPI_Transmit(&hspi1, &prefix, sizeof(prefix), 100);
    uint8_t line_command = 0x45;
    HAL_SPI_Transmit(&hspi1, &line_command, sizeof(line_command), 100);
    uint8_t row_pos = 0x40 * (row - 1); //row is line 1 / line 2
    uint8_t position = row_pos + column - 1;
    HAL_SPI_Transmit(&hspi1, &position, sizeof(position), 100);


}
