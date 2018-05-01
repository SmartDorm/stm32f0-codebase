/*
 * bluetooth_client.c
 *
 *  Created on: Apr 26, 2018
 *      Author: chili
 */

// DEFINING THE LANGUAGE
// MS -> music
// CC -> Connect
// TM -> Alarm
#define CONNECT "CC"
#define CONNECT_VALID "SD"
#define ALARM "TM"
#define ADD "AD"
#define DELETE "DL"
#define MUSIC "MS"
#define PLAY "PL"
#define STOP "ST"

//fields separated by _, use strtok
#include "smart_dorm.h"
#include "bluetooth_client.h"
#include "alarm.h"
#include <string.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void interpret_command(char * cw) {

    char * token;
    token = strtok(cw, ":_\r\n");
    if(!strcmp(token, CONNECT)) {
        token = strtok(NULL, ":_\r\n");
        if(!strcmp(token, CONNECT_VALID)) {
            transmit_bluetooth("CC_CD_Meyer\r\n");
        }
    } else if(!strcmp(token, ALARM)) {
        token = strtok(NULL, ":_\r\n");
        if(!strcmp(token, ADD)) {
            token = strtok(NULL, ":_\r\n"); // SHOULD give hours
            int hr = atoi(token);
            token = strtok(NULL, ":_\r\n"); // SHOULD give minutes
            int min = atoi(token);
            alarm_add(hr, min);
        } else if(!strcmp(token, DELETE)) {
            token = strtok(NULL, ":_\r\n"); // SHOULD give hours
            int hr = atoi(token);
            token = strtok(NULL, ":_\r\n"); // SHOULD give minutes
            int min = atoi(token);
            alarm_delete(hr, min);
        }
    } else if (!strcmp(token, MUSIC)) {
        token = strtok(NULL, ":_\r\n");
        if(!strcmp(token, PLAY)) {
            HAL_UART_Transmit(&huart1, (uint8_t *)"MUSC", 4, 10);
            HAL_Delay(10);
        } else if(!strcmp(token, STOP)) {
            HAL_UART_Transmit(&huart1, (uint8_t *) "KILL", 4, 10);
            HAL_Delay(10);
        }
    }
}

void transmit_bluetooth(char * word) {
    int length = strlen(word);
    HAL_UART_Transmit(&huart2, word, length, 100);
}
