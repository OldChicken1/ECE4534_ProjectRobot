/*
 * my_spi.h
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#ifndef SPI_HAL_H_
#define SPI_HAL_H_

#include <stddef.h>
#include <string.h>
#include <ti/drivers/SPI.h>
#include "Board.h"
#include <unistd.h>

#define ENCODER_COUNT_LENGTH 5
#define ENCODER_MODE_LENGTH 2
#define ENCODER_CLEAR_LENGTH 5
#define ENCODER_TRANSFER_LENGTH 1

SPI_Handle init_spi();

int spi_read_encoder(SPI_Handle* spiHandle, uint8_t *byte0, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3);

int spi_init_encoder_mode(SPI_Handle* spiHandle);

int spi_clear_encoder(SPI_Handle* spiHandle);

int spi_transfer_cleared_encoder(SPI_Handle* spiHandle);

int transfer_msg(SPI_Handle* spiHandle, uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frameCount);

#endif /* SPI_HAL_H_ */
