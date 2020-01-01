/*
 * navigation_queue.c
 *
 *  Created on: Nov 5, 2019
 *      Author: lizid
 */

#include "navigation_queue.h"

QueueHandle_t createQueue_Navigation() {
    xQueue_Navigation = xQueueCreate(Navigation_QUEUE_LENGTH, Navigation_QUEUE_WIDTH);
    return xQueue_Navigation;
}

void sendTimerMSGToNavigationQ(int isInISR){
    NavigationMessage pxmsg = {0, 0, 6};

    if (isInISR) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xQueueSendFromISR(xQueue_Navigation, (void *)&pxmsg, &xHigherPriorityTaskWoken);
    }
    else {
        xQueueSend(xQueue_Navigation, (void *)&pxmsg, 0);
    }
}
//void sendMoveForwardMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Move_Forward_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendMoveBackwardMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Move_Backward_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendMoveLeftMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Move_Left_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendMoveRightMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Move_Right_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendRotateLeftMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Rotate_Left_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendRotateRightMsgToNavigationQ(uint32_t speed) {
//    NavigationMessage pxmsg = {Navigation_Rotate_Right_MSG_ID, speed, 0, 0, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
//void sendRotateTimeMsgToNavigationQ(uint16_t CW, uint16_t speed, uint16_t time) {
//    NavigationMessage pxmsg = {Navigation_RotateTime_MSG_ID, CW , speed, time, 0, 0, 0};
//
//    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
//}
//
void sendStopMsgToNavigationQ() {
    NavigationMessage pxmsg = {Navigation_Stop_MSG_ID};

    xQueueSend(xQueue_Navigation, (void *)&pxmsg, portMAX_DELAY);
}

void sendMSGToNavigationQ(int inISR, int angle, uint32_t distance, int moveType) {
    NavigationMessage pxmsg = {angle, distance, moveType};

    if (inISR) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        xQueueSendFromISR(xQueue_Navigation, (void *)&pxmsg, &xHigherPriorityTaskWoken);
    }
    else {
        xQueueSend(xQueue_Navigation, (void *)&pxmsg, 0);
    }
}

int receiveMsgFromNavigationQ(Movement_Type* movement) {
    int success = 1;
    NavigationMessage pxrecmsg;

    xQueueReceive(xQueue_Navigation, &pxrecmsg, portMAX_DELAY);
    movement->angle = pxrecmsg.angle;
    movement->distance = pxrecmsg.distance;
    movement->moveType = pxrecmsg.moveType;
    return success;
}
