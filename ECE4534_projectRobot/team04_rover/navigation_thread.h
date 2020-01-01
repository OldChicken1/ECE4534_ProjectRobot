/*
 * navigation_thread.h
 *
 *  Created on: Nov 5, 2019
 *      Author: lizid
 */

#ifndef NAVIGATION_THREAD_H_
#define NAVIGATION_THREAD_H_

#include "navigation_queue.h"
#include "control_queue.h"
#include "timer_HAL.h"
#include "debug_queue.h"

#define Default_Speed 30
#define KP            0.5
#define DISTKP        0.08
#define KI            0.05
#define TARGETDIST    10
#define ROTATION      0
#define FORWARD       1
#define FINDING       2
#define ARM_GRAB        3
#define STOP          4
#define ROTATETOBACK  5
#define TIMERFINISH   6
void *navigation_threadFxn(void *arg0);
void angleToSpeed(int64_t speed);
void distToSpeed(int64_t speed);

#endif /* NAVIGATION_THREAD_H_ */
