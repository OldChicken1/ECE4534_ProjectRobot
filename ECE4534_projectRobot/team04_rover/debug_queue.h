/*
 * debug_queue.h
 *
 *  Created on: Oct 9, 2019
 *      Author: lizid
 */

#ifndef DEBUG_QUEUE_H_
#define DEBUG_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include "control_queue.h"
#include <stdio.h>
#include <string.h>
#include "uart_term.h"

#define Debug_Time_MSG_ID  0
#define Button_Pressed_MSG_ID  1
#define ERROR_MSG_ID  2
#define Debug_Wheel0_Speed_MSG_ID 3
#define Debug_Wheel1_Speed_MSG_ID 4
#define Debug_Wheel2_Speed_MSG_ID 5

typedef struct {
    uint8_t msgID;
    uint32_t message;
} DebugMessage;

#define Debug_QUEUE_LENGTH 10
#define Debug_QUEUE_WIDTH sizeof(DebugMessage)

QueueHandle_t xQueue_Debug;

QueueHandle_t createQueue_DebugQ();

int sendWheel0SpeedMsgToDebugQ(uint32_t speed);

int sendWheel1SpeedMsgToDebugQ(uint32_t speed);

int sendWheel2SpeedMsgToDebugQ(uint32_t speed);

int sendButtonPressedMsgToDebugQ();

int sendTimeMsgToDebugQ();

int error_flag();

int receiveMsgFromDebugQ(int* error, int* button_pressed, int* timeVal);

#endif /* DEBUG_QUEUE_H_ */
