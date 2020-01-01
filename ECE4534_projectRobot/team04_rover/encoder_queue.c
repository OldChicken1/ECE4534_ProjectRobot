/*
 * sensor_queue.c
 *
 *  Created on: Sep 6, 2019
 *      Author: mmcgruther
 */
#include <encoder_queue.h>

QueueHandle_t createQueue_Encoder()
{
    xQueue_Encoder = xQueueCreate(En_QUEUE_LENGTH, En_QUEUE_WIDTH);
    return xQueue_Encoder;
}

int sendTimeMsgToEncoderQ()
{
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EncoderMessage pxmsg = {TIME_MSG_ID, 1};

    xQueueSendFromISR(xQueue_Encoder, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int sendStartTimerMsgToEncoderQ() {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    EncoderMessage pxmsg = {Start_Timer_MSG_ID, 0};

    xQueueSendFromISR(xQueue_Encoder, (void *)&pxmsg, &xHigherPriorityTaskWoken);

    return success;
}

int receiveMsgFromEncoderQ(int *timeVal, int* start_timer0)
{
    int success = 1;
    EncoderMessage pxrecmsg;

    xQueueReceive(xQueue_Encoder, &pxrecmsg, portMAX_DELAY);

    switch (pxrecmsg.msgID)
    {
    case TIME_MSG_ID:
        *timeVal = 1;
        break;
    case Start_Timer_MSG_ID:
        *start_timer0 = 1;
        break;
    default:
        success = 0;
        break;
    }
    return success;
}
