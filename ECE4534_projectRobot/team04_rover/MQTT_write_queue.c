/*
 * sensor_queue.c
 *
 *  Created on: Sep 6, 2019
 *      Author: mmcgruther
 */
#include <MQTT_write_queue.h>

QueueHandle_t createQueueMWQ()
{
    xQueueMWQ = xQueueCreate(MWQ_QUEUE_LENGTH, MWQ_QUEUE_WIDTH);
    return xQueueMWQ;
}

void sendStatMsgToMWQ_BL(uint16_t publish, uint16_t receive, uint16_t miss)
{
    // BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    MWQMessage pxmsg = {MWQ_STAT_MSG_ID, (publish<<16)|receive,miss, 0};

    if(xQueueSend(xQueueMWQ, (void *)&pxmsg, portMAX_DELAY) != pdTRUE)
    {
        setErrorFlagTrue();
    }
}

void sendWheelsSpeedToMQTT(uint32_t speed1, uint32_t speed2, uint32_t speed3) {
    MWQMessage pxmsg = {MWQ_SPEED_MSG_ID, speed1, speed2, speed3};

    if(xQueueSend(xQueueMWQ, (void *)&pxmsg, portMAX_DELAY) != pdTRUE)
    {
        setErrorFlagTrue();
    }
}

void sendStillToMQTT(uint16_t still) {
    MWQMessage pxmsg = {MWQ_STILL_MSG_ID, still, 0, 0};

    if(xQueueSend(xQueueMWQ, (void *)&pxmsg, portMAX_DELAY) != pdTRUE)
    {
        setErrorFlagTrue();
    }
}

//void sendDebugToMQTT(uint32_t)

void receiveMsgFromMWQ_BL(uint16_t *topic_index_p, uint16_t *publishVal_p, uint16_t *receiveVal_p, uint16_t *missVal_p, uint32_t *speed1, uint32_t *speed2, uint32_t *speed3, uint32_t *isStill)
{
    MWQMessage pxrecmsg;

    xQueueReceive(xQueueMWQ, &pxrecmsg, portMAX_DELAY);

    char msgID = pxrecmsg.msgID;
    switch (msgID)
    {
    case MWQ_STAT_MSG_ID:
        *publishVal_p = pxrecmsg.message1>>16;
        *receiveVal_p=pxrecmsg.message1;
        *missVal_p=pxrecmsg.message2;
        *topic_index_p = msgID;
        break;
    case MWQ_SPEED_MSG_ID:
        *topic_index_p = msgID;
        *speed1 = pxrecmsg.message1;
        *speed2 = pxrecmsg.message2;
        *speed3 = pxrecmsg.message3;
        break;
    case MWQ_STILL_MSG_ID:
        *topic_index_p = msgID;
        *isStill = pxrecmsg.message1;
        break;
    default:
        setErrorFlagTrue();
    }
}
