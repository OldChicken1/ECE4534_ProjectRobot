/*
 * control_queue.c
 *
 *  Created on: Sep 30, 2019
 *      Author: lizid
 */

#include "control_queue.h"

QueueHandle_t createQueue_Control() {
    xQueue_Control = xQueueCreate(Control_QUEUE_LENGTH, Control_QUEUE_WIDTH);
    return xQueue_Control;
}

// Speed Functions
int sendWheels_SpeedMsgToControlQ(uint32_t speed1, uint32_t speed2, uint32_t speed3) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel_Speed_MSG_ID, speed1, speed2, speed3};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

// Movement Functions
int sendWheel0ForwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel0_Move_Forward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);
    //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //xQueueSendFromISR(xQueue_Control, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int sendWheel0BackwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel0_Move_Backward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int sendWheel1ForwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel1_Move_Forward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);
    //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //xQueueSendFromISR(xQueue_Control, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int sendWheel1BackwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel1_Move_Backward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int sendWheel2ForwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel2_Move_Forward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);
    //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //xQueueSendFromISR(xQueue_Control, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int sendWheel2BackwardMsgToControlQ(uint32_t speed) {
    int success = 1;
    ControlMessage pxmsg = {Control_Wheel2_Move_Backward_MSG_ID, speed, 0, 0};

    xQueueSend(xQueue_Control, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

// Receive Function
int receiveMsgFromControlQ(Wheel_Speed_Type* speed, Wheel_Move_Type* move0, Wheel_Move_Type* move1, Wheel_Move_Type* move2) {
    int success = 1;
    ControlMessage pxrecmsg;

    xQueueReceive(xQueue_Control, &pxrecmsg, portMAX_DELAY);

    switch (pxrecmsg.msgID)
    {
    case Control_Wheel_Speed_MSG_ID:
        speed->updated = 1;
        speed->speed1 = pxrecmsg.message1;
        speed->speed2 = pxrecmsg.message2;
        speed->speed3 = pxrecmsg.message3;
        break;
    case Control_Wheel0_Move_Forward_MSG_ID:
        move0->updated = 1;
        move0->direction = Command_Forward;
        move0->speed = pxrecmsg.message1;
        break;
    case Control_Wheel0_Move_Backward_MSG_ID:
        move0->updated = 1;
        move0->direction = Command_Backward;
        move0->speed = pxrecmsg.message1;
        break;
    case Control_Wheel1_Move_Forward_MSG_ID:
        move1->updated = 1;
        move1->direction = Command_Forward;
        move1->speed = pxrecmsg.message1;
        break;
    case Control_Wheel1_Move_Backward_MSG_ID:
        move1->updated = 1;
        move1->direction = Command_Backward;
        move1->speed = pxrecmsg.message1;
        break;
    case Control_Wheel2_Move_Forward_MSG_ID:
        move2->updated = 1;
        move2->direction = Command_Forward;
        move2->speed = pxrecmsg.message1;
        break;
    case Control_Wheel2_Move_Backward_MSG_ID:
        move2->updated = 1;
        move2->direction = Command_Backward;
        move2->speed = pxrecmsg.message1;
        break;
    default:
        success = 0;
        break;
    }
    return success;
}
