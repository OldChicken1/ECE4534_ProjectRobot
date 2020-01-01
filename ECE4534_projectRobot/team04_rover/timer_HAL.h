/*
 * timerone.h
 *
 *  Created on: Sep 7, 2019
 *      Author: mmcgr
 */

#ifndef TIMER_HAL_H_
#define TIMER_HAL_H_

#define TIMER_ONE_PERIOD        100000
#define TIMER_TWO_PERIOD        1000000
#define DISTANCE_TIMER_PERIOD   10
#define ROTATION_TIMER_PERIOD    8000

#include <ti/drivers/Timer.h>
#include "encoder_queue.h"
#include "debug_queue.h"
#include "navigation_queue.h"
#include "Board.h"
#include <timers.h>
#include <ti/drivers/dpl/HwiP.h>

Timer_Handle timer0Init();

void timer0Callback(Timer_Handle myHandle);

Timer_Handle timer1Init();

void timer1Callback(Timer_Handle myHandle);

TimerHandle_t rotation_timer_Init();

void rotationTimerCallback( TimerHandle_t xTimer);


#endif /* TIMER_HAL_H_ */
