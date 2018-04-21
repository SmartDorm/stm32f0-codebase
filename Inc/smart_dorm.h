/*
 * smart_dorm.h
 *
 *  Created on: Apr 14, 2018
 *      Author: chili
 */

#ifndef SMART_DORM_H_
#define SMART_DORM_H_

// typedefs
typedef enum {false = 0, true} bool;
typedef enum {idle, music, connect, err} State;

// function definitions
void app_main();
void init();
State idle_s();
void fetch(char * command);

#endif /* SMART_DORM_H_ */

/*
 * State machine model of smart dorm:
 *
 * STATE Idle:
 *   -Display time
 *   -Switch through temperatures & other passive information at set intervals
 *
 *   State changes:
 *      Start with USART Receive interrupt listener
 *      On interrupt: change state to Phone connect
 *
 * STATE Phone connect:
 *   -Receive command from phone
 *   -Change LED levels
 *   -Possible state changes: Music
 *
 * STATE Music:
 *   -Receive music data from phone, put through to speakers
 *   -Alter volume
 */
