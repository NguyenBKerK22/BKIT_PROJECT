/*
 * timer.h
 *
 *  Created on: Dec 14, 2024
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_


#define TICK 1 // ms

struct TimerStruct
{
	int counter;
	int flag;
};

extern struct TimerStruct timer[10];

void setTimer(int idx, int counter);
void timerRun();
int isFlag(int idx);
int msToTick(int time_ms);

#endif /* INC_TIMER_H_ */
