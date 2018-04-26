/*
 * alarm.h
 *
 *  Created on: Apr 24, 2018
 *      Author: jared
 */

#ifndef ALARM_H_
#define ALARM_H_

void update_alarm_value(int indx);

void stop_alarm();

void speaker_off();

int alarm_control(int alarm_counter);

void alarm_search(int hour, int minute);
void alarm_add(int hour, int minute);
void alarm_delete(int hour, int minute);
void alarm_trigger();

#endif /* ALARM_H_ */



