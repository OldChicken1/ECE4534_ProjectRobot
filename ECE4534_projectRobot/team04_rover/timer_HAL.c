/*
 * timerone.c
 *
 *  Created on: Sep 7, 2019
 *      Author: mmcgr
 */

#include <timer_HAL.h>

Timer_Handle timer0Init()
{
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = TIMER_ONE_PERIOD;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timer0Callback;

    return Timer_open(Board_TIMER0, &params);
}

void timer0Callback(Timer_Handle myHandle)
{
    if(!sendTimeMsgToEncoderQ())
    {
        //setErrorFlagTrue();
    }
}

Timer_Handle timer1Init()
{
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = TIMER_TWO_PERIOD;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_ONESHOT_CALLBACK;
    params.timerCallback = timer1Callback;

    return Timer_open(Board_TIMER1, &params);
}

void timer1Callback(Timer_Handle myHandle)
{
    if(!(sendTimeMsgToDebugQ()))
    {
        //setErrorFlagTrue();
    }
}

TimerHandle_t rotation_timer_Init() {
    TimerHandle_t xTimer = xTimerCreate("Rotation_Timer", ROTATION_TIMER_PERIOD, pdTRUE, ( void * ) 0, rotationTimerCallback);
    if( xTimer == NULL )
    {
       /* The timer was not created. */
    }

    return xTimer;
}

void rotationTimerCallback( TimerHandle_t xTimer) {
    sendTimerMSGToNavigationQ(HwiP_inISR());
}
