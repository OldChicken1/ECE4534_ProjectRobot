/*
 * navigation_queue.h
 *
 *  Created on: Nov 5, 2019
 *      Author: lizid
 */

#ifndef NAVIGATION_QUEUE_H_
#define NAVIGATION_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

#define Navigation_Move_Forward_MSG_ID         4
#define Navigation_Move_Backward_MSG_ID         5
#define Navigation_Move_Left_MSG_ID         6
#define Navigation_Move_Right_MSG_ID         7
#define Navigation_Rotate_Left_MSG_ID         8
#define Navigation_Rotate_Right_MSG_ID         9
#define Navigation_Stop_MSG_ID          100
#define Navigation_RotateTime_MSG_ID    10

#define Navigation_cwTime_MSG     1
#define Navigation_ccwTime_MSG     0

#define Navigation_Start_Timer_MSG_ID   50

#define Navigation_Timer_MSG_ID     11
#define Navigation_Wheels_MSG_ID    12

typedef enum {forward, backward, left, right, clockwise, counterclockwise, cwTime, ccwTime, none} Rover_Direction_Type;

typedef struct {
//    uint8_t msgID;
//    uint32_t message;
//    uint32_t message1;
//    uint32_t message2;
//    uint32_t message3;
//    uint32_t message4;
//    uint32_t message5;
    int angle;
    uint32_t distance;
    int moveType;
} NavigationMessage;


#define Navigation_QUEUE_LENGTH        10
#define Navigation_QUEUE_WIDTH         sizeof(NavigationMessage)

typedef struct {
//    int updated;
//    Rover_Direction_Type direction;
//    uint32_t speed;
//    uint32_t time;
    int angle;
    uint32_t distance;
    int moveType;
} Movement_Type;

typedef struct {
    int updated;
    uint32_t direction1;
    uint32_t speed1;
    uint32_t direction2;
    uint32_t speed2;
    uint32_t direction3;
    uint32_t speed3;
} Wheels_Movement_t;

QueueHandle_t xQueue_Navigation;

QueueHandle_t createQueue_Navigation();

void sendMSGToNavigationQ(int inISR, int angle, uint32_t distance, int moveType);
int receiveMsgFromNavigationQ(Movement_Type* movement);
void sendStopMsgToNavigationQ();
#endif /* NAVIGATION_QUEUE_H_ */
