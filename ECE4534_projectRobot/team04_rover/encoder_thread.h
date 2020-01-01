/*
 * encoder_thread.h
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#ifndef ENCODER_THREAD_H_
#define ENCODER_THREAD_H_

#include "SPI_HAL.h"
#include <ti/drivers/GPIO.h>
#include <unistd.h>
#include "encoder_queue.h"
#include "control_queue.h"
#include "MQTT_write_queue.h"
#include "timer_HAL.h"

#define Max_Diff 0x0FFFFFFF
#define MAX_32BITS 0xFFFFFFFF
#define TIME_FREQUENCY_HZ 10

void *encoder_threadFxn(void *arg0);

int init_encoder(SPI_Handle* spiHandle, unsigned index);

uint32_t read_encoder(SPI_Handle* spiHandle, unsigned index);

uint32_t getSpeed(uint32_t prev_counts, uint32_t counts);

#endif /* ENCODER_THREAD_H_ */
