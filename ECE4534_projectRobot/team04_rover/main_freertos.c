/*
 * Copyright (c) 2016-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== main_freertos.c ========
 */
#include <stdint.h>

#ifdef __ICCARM__
#include <DLib_Threads.h>
#endif

/* POSIX Header files */
#include <pthread.h>

/* RTOS header files */
#include <FreeRTOS.h>
#include <task.h>

/* Example/Board Header files */
#include <ti/drivers/Board.h>
#include "encoder_thread.h"
#include "control_thread.h"
#include "MQTT_write_queue.h"
#include "stat_queue.h"
#include "navigation_queue.h"
#include "timer_HAL.h"

//extern void *mainThread(void *arg0);

/* Stack size in bytes */
//#define THREADSTACKSIZE   1024
extern void * mainThread(void *arg0);

extern void * MQTT_test_Thread(void *arg0);

extern void * stat_Thread(void *arg0);

extern void * encoder_threadFxn(void *arg0);

extern void * control_threadFxn(void *arg0);

extern void * debug_threadFxn(void *arg0);

extern void * navigation_threadFxn(void *arg0);

#define THREADSTACKSIZE   4096

/*
 *  ======== main ========
 */
int main(void)
{
    pthread_t           thread0, thread2, thread01, thread11, thread12, thread13;
    pthread_attr_t      attrs;
    struct sched_param  priParam;
    int                 retc;

    /* initialize the system locks */
#ifdef __ICCARM__
    __iar_Initlocks();
#endif

    /* Call driver init functions */
    Board_init();
    GPIO_init();
    Timer_init();
    UART_init();
    SPI_init();

    // create message queues
    if (createQueue_Encoder() == NULL) {
        while (1);
    }

    if (createQueue_Control() == NULL) {
        while (1);
    }

    if (createQueue_DebugQ() == NULL) {
        while (1);
    }

    if (createQueueMWQ() == NULL) {
        while (1);
    }

    if (createQueueMSQ() == NULL) {
        while (1);
    }

    if (createQueue_Navigation() == NULL) {
        while (1);
    }

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 1;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0) {
        /* failed to set attributes */
        while (1) {}
    }

    /**
     * create MQTT thread
     */
    retc = pthread_create(&thread0, &attrs, mainThread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /**
     * create statistics thread
     */
    retc = pthread_create(&thread2, &attrs, stat_Thread, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1) {}
    }

    /**
     * create encoder thread
     */
    retc = pthread_create(&thread01, &attrs, encoder_threadFxn, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /**
     * Create control thread
     */
    retc = pthread_create(&thread11, &attrs, control_threadFxn, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /**
     * Create navigation thread
     */
    retc = pthread_create(&thread13, &attrs, navigation_threadFxn, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }

    /**
     * Create Debug thread
     */
    retc = pthread_create(&thread12, &attrs, debug_threadFxn, NULL);
    if (retc != 0) {
        /* pthread_create() failed */
        while (1);
    }


    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    return (0);
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook()
{
    /* Handle Memory Allocation Errors */
    while(1)
    {
    }
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while(1)
    {
    }
}
