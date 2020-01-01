/*
 * control_queue.h
 *
 *  Created on: Sep 30, 2019
 *      Author: lizid
 */

#ifndef CONTROL_QUEUE_H_
#define CONTROL_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

typedef struct {
    uint8_t msgID;
    uint32_t message1;
    uint32_t message2;
    uint32_t message3;
} ControlMessage;

typedef struct {
    int updated;
    uint32_t speed1;
    uint32_t speed2;
    uint32_t speed3;
} Wheel_Speed_Type;


typedef struct {
    int updated;
    uint32_t speed;
    uint8_t direction;
} Wheel_Move_Type;


#define Control_QUEUE_LENGTH        10
#define Control_QUEUE_WIDTH         sizeof(ControlMessage)

#define Control_Wheel_Speed_MSG_ID         100
#define Control_Wheel0_Speed_MSG_ID     1
#define Control_Wheel1_Speed_MSG_ID     2
#define Control_Wheel2_Speed_MSG_ID     3

#define Control_Wheel0_Move_Forward_MSG_ID         10
#define Control_Wheel0_Move_Backward_MSG_ID         11
#define Control_Wheel1_Move_Forward_MSG_ID         12
#define Control_Wheel1_Move_Backward_MSG_ID         13
#define Control_Wheel2_Move_Forward_MSG_ID         14
#define Control_Wheel2_Move_Backward_MSG_ID         15


#define Command_Forward 0
#define Command_Backward 1

/*
 * Queue handles defined as global vars, per Code Requirements Documentation
 */
QueueHandle_t xQueue_Control;

/*
 * Initializes Q1 with length = QUEUE_LENGTH and width = QUEUE_WIDTH
 */
QueueHandle_t createQueue_Control();
/*
 * Sends speed message to queue using nonblocking *fromISR send
 */
int sendWheels_SpeedMsgToControlQ(uint32_t speed1, uint32_t speed2, uint32_t speed3);

// wheel movement
int sendWheel0ForwardMsgToControlQ(uint32_t speed);

int sendWheel0BackwardMsgToControlQ(uint32_t speed);

int sendWheel1ForwardMsgToControlQ(uint32_t speed);

int sendWheel1BackwardMsgToControlQ(uint32_t speed);

int sendWheel2ForwardMsgToControlQ(uint32_t speed);

int sendWheel2BackwardMsgToControlQ(uint32_t speed);

/*
 * Blocking receive from Q1. Determines data type (time/sensor) and assigns received value to
 *  appropriate argument. Sets other arg = 0.
 */
int receiveMsgFromControlQ(Wheel_Speed_Type* speed, Wheel_Move_Type* move0, Wheel_Move_Type* move1, Wheel_Move_Type* move2);


#endif /* CONTROL_QUEUE_H_ */
