/*
 * my_spi.c
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#include "SPI_HAL.h"

SPI_Handle init_spi() {
    SPI_Params spiParams;
    // SPI_init();
    SPI_Params_init(&spiParams);
    //spiParams.frameFormat = SPI_POL0_PHA0;  // SPICLK is active high and samples on the falling edge
    spiParams.bitRate = 50000;
    return SPI_open(Board_SPI0, &spiParams);
}

int spi_init_encoder_mode(SPI_Handle* spiHandle) {
    /*
     * write to MDR0 reg, set mode in MDR0  0x88
     *
     * B1 B0 = 11: x4  quadrature count mode
     * B3 B2 = 00: free running
     * B5 B4 = 00: disable index
     * B6 = 0: Asynchronous Index
     * B7 = 0: Filter clock division factor = 1
     * 0x03
     */

    uint8_t TXmodeMSG[] = {0x88, 0x03};
    uint8_t RXmodeMSG[] = {0, 0};  // junk
    return transfer_msg(spiHandle, RXmodeMSG, TXmodeMSG, sizeof(TXmodeMSG));
}

int spi_read_encoder(SPI_Handle* spiHandle, uint8_t *byte0, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3) {
    /*
     * read from CNTR, 0x60, 4 bytes
     */
    uint8_t TXcountMSG[] = {0x60, 0, 0, 0, 0}; // 4 junks
    uint8_t RXcountMSG[] = {0, 0, 0, 0, 0};  // junk
    int transferOK = transfer_msg(spiHandle, RXcountMSG, TXcountMSG, sizeof(TXcountMSG));

    *byte0 = RXcountMSG[4];
    *byte1 = RXcountMSG[3];
    *byte2 = RXcountMSG[2];
    *byte3 = RXcountMSG[1];
/*
    if (*byte3 > 0xf0) {
        int i = 111;
    }*/

    return transferOK;
}

int spi_clear_encoder(SPI_Handle* spiHandle) {
    /**
     * IR = 0x98: write to DTR
     */
    uint8_t TXclearMSG[] = {0x20};
    uint8_t RXclearMSG[] = {0}; // junk
    return transfer_msg(spiHandle, RXclearMSG, TXclearMSG, sizeof(TXclearMSG));
}

int spi_transfer_cleared_encoder(SPI_Handle* spiHandle) {
    // transfer DTR to counter
    // IR: Transfer DTR to CNTR
    uint8_t TXtransferMSG[] = {0xE0, 0, 0, 0, 0};
    uint8_t RXtransferMSG[] = {0, 0, 0, 0, 0}; // junk
    return transfer_msg(spiHandle, RXtransferMSG, TXtransferMSG, sizeof(TXtransferMSG));
}

int transfer_msg(SPI_Handle* spiHandle, uint8_t *rx_buffer, uint8_t *tx_buffer, unsigned frameCount) {
    SPI_Transaction transaction;
    transaction.count = frameCount;
    transaction.txBuf = (void *) tx_buffer;
    transaction.rxBuf = (void *) rx_buffer;
    return SPI_transfer(*spiHandle, &transaction);
}
