/*
 * sensor_queue.h
 *
 *  Created on: Sep 6, 2019
 *      Author: mmcgruther
 */

#ifndef ENCODER_QUEUE_H_
#define ENCODER_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include "debug_rover.h"

typedef struct {
    uint8_t msgID;
    uint32_t message;
} EncoderMessage;

#define En_QUEUE_LENGTH        10
#define En_QUEUE_WIDTH         sizeof(EncoderMessage)

#define TIME_MSG_ID         1
#define Start_Timer_MSG_ID  2

/*
 * Queue handles defined as global vars, per Code Requirements Documentation
 */
QueueHandle_t xQueue_Encoder;

/*
 * Initializes Q1 with length = QUEUE_LENGTH and width = QUEUE_WIDTH
 */
QueueHandle_t createQueue_Encoder();

/*
 * Sends time message to queue using nonblocking *fromISR send
 */
int sendTimeMsgToEncoderQ();

/*
 * Sends start timer signal
 */
int sendStartTimerMsgToEncoderQ();

/*
 * Blocking receive from Q1. Determines data type (time/sensor) and assigns received value to
 *  appropriate argument. Sets other arg = 0.
 */
int receiveMsgFromEncoderQ(int *timeVal, int* start_timer0);

#endif /* ENCODER_QUEUE_H_ */
