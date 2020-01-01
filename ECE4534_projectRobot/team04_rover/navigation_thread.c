/*
 * navigation_thread.c
 *
 *  Created on: Nov 5, 2019
 *      Author: lizid
 */

#include "navigation_thread.h"

void angleToSpeed(int64_t speed)
{
    uint32_t speedUpdate;
    if (speed >= 0)
    {
        speedUpdate = (uint32_t) speed;
        sendWheel0BackwardMsgToControlQ(speedUpdate);
        sendWheel1BackwardMsgToControlQ(speedUpdate);
        sendWheel2BackwardMsgToControlQ(speedUpdate);
    }
    else
    {
        speedUpdate = (uint32_t) (-1 * speed);
        sendWheel0ForwardMsgToControlQ(speedUpdate);
        sendWheel1ForwardMsgToControlQ(speedUpdate);
        sendWheel2ForwardMsgToControlQ(speedUpdate);
    }
}
void distToSpeed(int64_t speed)
{
    uint32_t speedUpdate;
    if (speed >= 0)
    {
        speedUpdate = (uint32_t) speed;
        sendWheel2ForwardMsgToControlQ(0);
        sendWheel0ForwardMsgToControlQ(10);
        sendWheel1BackwardMsgToControlQ(10);
    }
    else
    {
        speedUpdate = (uint32_t) (-1 * speed);
        sendWheel2BackwardMsgToControlQ(0);
        sendWheel0BackwardMsgToControlQ(10);
        sendWheel1ForwardMsgToControlQ(10);
    }
}

void *navigation_threadFxn(void *arg0)
{

    Movement_Type movement;
//    Wheels_Movement_t wheels_movement;
    TimerHandle_t rotation_timer = rotation_timer_Init();
//    int timer_expired = 0;
    int isTurning = 0;
    while (1)
    {
        int64_t speedError;
        int64_t distError;
        if (receiveMsgFromNavigationQ(&movement))
        {
            if(movement.moveType == TIMERFINISH)
            {
  //                xTimerStop(rotation_timer, 0);
                if(isTurning == 2){
//                    distToSpeed(0);
                    angleToSpeed(40);
                    isTurning = 1;
                }
                else if(isTurning == 1){
                    angleToSpeed(0);
                    sendStillToMQTT(1);
                    xTimerStop(rotation_timer, 0);
                    isTurning = 0;
                }

            }
            else if (!isTurning)
            {
                switch (movement.moveType)
                {
                case ROTATION:
                    //                in += movement.angle;
                    //                speedError = KP * movement.angle + KI * in;
                    speedError = KP * movement.angle;
                    angleToSpeed(speedError);
                    break;
                case FORWARD:
                    distError = TARGETDIST - movement.distance;
                    distToSpeed(distError);
                    break;
                case FINDING:
                    angleToSpeed(10);
                    break;
                case ARM_GRAB:
                    angleToSpeed(0);
                    break;
                case STOP:
                    angleToSpeed(0);
                    break;
                case ROTATETOBACK:
                    xTimerStart(rotation_timer, 0);
                    isTurning = 2;
                    distToSpeed(-5);
                    break;
                default:
                    break;
                }
            }

//            if(movement.angle > -2 && movement.angle < 2)
//            {
//
//            }
//            else
//            {

//            }
//            if (timer_expired) {
//                sendWheel0ForwardMsgToControlQ(0);
//                sendWheel1ForwardMsgToControlQ(0);
//                sendWheel2ForwardMsgToControlQ(0);
//                timer_expired = 0;
//            }
//            else if (movement.updated) {
//                switch (movement.direction) {
//                case forward:
//                    sendWheel0ForwardMsgToControlQ(0);
//                    sendWheel1BackwardMsgToControlQ(movement.speed);
//                    sendWheel2ForwardMsgToControlQ(movement.speed);
//                    break;
//                case backward:
//                    sendWheel0ForwardMsgToControlQ(0);
//                    sendWheel1ForwardMsgToControlQ(movement.speed);
//                    sendWheel2BackwardMsgToControlQ(movement.speed);
//                    break;
//                case left:
//                    sendWheel0ForwardMsgToControlQ(movement.speed);
//                    sendWheel1BackwardMsgToControlQ(movement.speed / 2);
//                    sendWheel2BackwardMsgToControlQ(movement.speed / 2);
//                    break;
//                case right:
//                    sendWheel0BackwardMsgToControlQ(movement.speed);
//                    sendWheel1ForwardMsgToControlQ(movement.speed / 2);
//                    sendWheel2ForwardMsgToControlQ(movement.speed / 2);
//                    break;
//                case clockwise:
//                    sendWheel0BackwardMsgToControlQ(movement.speed);
//                    sendWheel1BackwardMsgToControlQ(movement.speed);
//                    sendWheel2BackwardMsgToControlQ(movement.speed);
//                    break;
//                case counterclockwise:
//                    sendWheel0ForwardMsgToControlQ(movement.speed);
//                    sendWheel1ForwardMsgToControlQ(movement.speed);
//                    sendWheel2ForwardMsgToControlQ(movement.speed);
//                    break;
//                case cwTime:
//                    sendWheel0BackwardMsgToControlQ(movement.speed);
//                    sendWheel1BackwardMsgToControlQ(movement.speed);
//                    sendWheel2BackwardMsgToControlQ(movement.speed);
//                    //xTimerStart(rotation_timer, portMAX_DELAY);
//                    xTimerChangePeriod(rotation_timer, movement.time, portMAX_DELAY);
//                    break;
//                case ccwTime:
//                    sendWheel0ForwardMsgToControlQ(movement.speed);
//                    sendWheel1ForwardMsgToControlQ(movement.speed);
//                    sendWheel2ForwardMsgToControlQ(movement.speed);
//                    //xTimerStart(rotation_timer, portMAX_DELAY);
//                    xTimerChangePeriod(rotation_timer, movement.time, portMAX_DELAY);
//                    break;
//                case none:
//                    sendWheel0ForwardMsgToControlQ(0);
//                    sendWheel1ForwardMsgToControlQ(0);
//                    sendWheel2ForwardMsgToControlQ(0);
//                    break;
//                default:
//                    break;
//                }
//                movement.updated = 0;
        }
//        else {
//            error_flag();
//        }
    }
}
