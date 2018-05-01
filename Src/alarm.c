/*
 * alarm.c
 *
 *  Created on: Apr 24, 2018
 *      Author: jared
 */

#include "smart_dorm.h"
#include "alarm.h"
#include "stm32f0xx_hal.h"

#define __RED_ON() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET)
#define MAX_ALARMS 8

extern DAC_HandleTypeDef hdac1;
extern TIM_HandleTypeDef htim3;
Alarm alarms[8];

void alarm_init() {
    for(int i = 0; i < 8; i++) {
        alarms[i].hour = -1;
        alarms[i].minute = -1;
    }
}

bool alarm_search(int hour, int minute) {
    for(int i = 0; i < MAX_ALARMS; i++) {
        if(hour == alarms[i].hour && minute == alarms[i].minute) {
            return true;
        }
    }
    return false;
}

void alarm_add(int hour, int minute) {
    for(int i = 0; i < MAX_ALARMS; i++) {
        if(alarms[i].hour == -1) {
            alarms[i].hour = hour;
            alarms[i].minute = minute;
            break;
        }
    }
}

void alarm_delete(int hour, int minute) {
    for(int i = 0; i < MAX_ALARMS; i++) {
        if(alarms[i].hour == hour && alarms[i].minute == minute) {
            alarms[i].hour = -1;
            alarms[i].minute = -1;
        }
    }
}

void update_alarm_value(int indx)
{
    uint32_t values[125] = {512, 537, 563, 588, 614, 639, 664, 688, 712, 735, 758, 780, 802, 823, 843, 862, 880, 898,
            914, 929, 944, 957, 969, 980, 990, 998, 100, 1012, 1017, 1020, 1022, 1023, 1023, 1022, 1019, 1014,
            1009, 1002, 994, 985, 975, 963, 951, 937, 922, 906, 889, 871, 852, 833, 812, 791, 769, 747, 724, 700,
            676, 651, 626, 601, 576, 550, 524, 499, 473, 447, 422, 397, 372, 347, 323, 299, 277, 254, 232, 211,
            190, 171, 152, 134, 117, 101, 86, 72, 60, 48, 38, 29, 21, 14, 9, 4, 1, 0, 0, 1, 3, 6, 11, 17, 25,
            33, 43, 54, 66, 79, 94, 109, 125, 143, 161, 180, 200, 221, 243, 265, 288, 311, 335, 359, 384, 409,
            435, 460, 486};

    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_L, values[indx] << 2);
}

void stop_alarm()
{
    //HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
    HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
}

void speaker_off()
{
    HAL_DAC_Stop(&hdac1, DAC_CHANNEL_1);
}

/*
 * this function will be placed within if(htim->Instance == TIM3)
 * to make sure that it is only activated by timer 3 in the interrupt callback
 * the input argument of alarm_counter is a count of 10 us
 * It returns an int because it will itself update the alarm counter
 */
int alarm_control(int alarm_counter)
{
    int alarm_index = alarm_counter % 125;
    update_alarm_value(alarm_index);
    alarm_counter++;

    if(alarm_counter == 125)
    {
        alarm_counter = 0;
    }

    return alarm_counter;
}
