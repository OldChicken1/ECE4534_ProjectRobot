/*
 * debug_queue.c
 *
 *  Created on: Oct 9, 2019
 *      Author: lizid
 */

#include "debug_queue.h"

QueueHandle_t createQueue_DebugQ() {
    xQueue_Debug = xQueueCreate(Debug_QUEUE_LENGTH, Debug_QUEUE_WIDTH);
    return xQueue_Debug;
}

int sendWheel0SpeedMsgToDebugQ(uint32_t speed){
    int success = 1;
    DebugMessage pxmsg = {Debug_Wheel0_Speed_MSG_ID, speed};

    xQueueSend(xQueue_Debug, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int sendWheel1SpeedMsgToDebugQ(uint32_t speed){
    int success = 1;
    DebugMessage pxmsg = {Debug_Wheel1_Speed_MSG_ID, speed};

    xQueueSend(xQueue_Debug, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int sendWheel2SpeedMsgToDebugQ(uint32_t speed){
    int success = 1;
    DebugMessage pxmsg = {Debug_Wheel2_Speed_MSG_ID, speed};

    xQueueSend(xQueue_Debug, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int sendButtonPressedMsgToDebugQ() {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    DebugMessage pxmsg = {Button_Pressed_MSG_ID, 1};

    xQueueSendFromISR(xQueue_Debug, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int sendTimeMsgToDebugQ() {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    DebugMessage pxmsg = {Debug_Time_MSG_ID, 1};

    xQueueSendFromISR(xQueue_Debug, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int error_flag() {
    int success = 1;
    DebugMessage pxmsg = {ERROR_MSG_ID, 1};

    xQueueSend(xQueue_Debug, (void *)&pxmsg, portMAX_DELAY);

    return success;
}

int receiveMsgFromDebugQ(int* error, int* button_pressed, int* timeVal) {
    int success = 1;
    DebugMessage pxrecmsg;

    xQueueReceive(xQueue_Debug, &pxrecmsg, portMAX_DELAY);

    switch (pxrecmsg.msgID)
    {
    case Debug_Time_MSG_ID:
        *timeVal = 1;
        break;
    case Button_Pressed_MSG_ID:
        *button_pressed = 1;
        break;
    case ERROR_MSG_ID:
        *error = 1;
        break;
    default:
        success = 0;
        break;
    }
    return success;
}

