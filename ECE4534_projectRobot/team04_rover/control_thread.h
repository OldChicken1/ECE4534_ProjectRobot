/*
 * control_thread.h
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#ifndef CONTROL_THREAD_H_
#define CONTROL_THREAD_H_

#include <stddef.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/UART.h>
#include <unistd.h>
#include "control_queue.h"
#include "encoder_queue.h"
#include "Board.h"
#include "Board.h"
#include "debug_rover.h"
#include "uart_term.h"

#define CHECKSUM_BITS 0b01111111
#define Controller1_addr 128
#define Controller2_addr 129
#define Controller3_addr 130
#define Byte_Length 1

// Kp = 0.3, Ki = 1
#define Wheel0_Kp   0.8
#define Wheel0_Ki   0.5
#define Wheel1_Kp   0.8
#define Wheel1_Ki   0.5
#define Wheel2_Kp   0.8
#define Wheel2_Ki   0.5
#define Time_Interval 1

void *control_threadFxn(void *arg0);

void initialize(UART_Handle* uart);

void init_wheel(Wheel_Move_Type* wheel);

void move_wheel(UART_Handle uart, uint8_t address, uint8_t direction, uint8_t speed);

uint8_t run_pi(int index, int64_t error, int64_t* integral, double Kp, double Ki, double dt);

uint8_t convertWheelSpeedToByte(int index, double speed);

#endif /* CONTROL_THREAD_H_ */
