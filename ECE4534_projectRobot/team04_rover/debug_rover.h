/*
 * debug.h
 *
 *  Created on: Oct 7, 2019
 *      Author: lizid
 */

#ifndef DEBUG_ROVER_H_
#define DEBUG_ROVER_H_

#include <stdbool.h>
#include <ti/drivers/GPIO.h>
#include "Board.h"
#include <ti/drivers/UART.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/Timer.h>
#include <ti/devices/cc32xx/inc/hw_gpio.h>
#include <ti/drivers/dpl/HwiP.h>
#include <pthread.h>
#include "debug_queue.h"
#include "control_queue.h"
#include "timer_HAL.h"

typedef enum {test0, test1, test2, test3, test4, test5, test6, test7, test8, test9, test10} test_state_Type;

void gpioButtonFxn0(uint_least8_t index);

void *debug_threadFxn(void *arg0);

void test_fxn(test_state_Type* test_state);

void impl_error_flag();


#endif /* DEBUG_ROVER_H_ */
