/*
 * encoder_thread.c
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#include "encoder_thread.h"

void *encoder_threadFxn(void *arg0) {
    GPIO_setConfig(Slave_Select0, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(Slave_Select1, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(Slave_Select2, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);
    SPI_Handle encoder_spi = init_spi();
    if (encoder_spi == NULL) {
        error_flag();
    }

    int status = init_encoder(&encoder_spi, Slave_Select0);
    status = init_encoder(&encoder_spi, Slave_Select1);
    status = init_encoder(&encoder_spi, Slave_Select2);

    if (!status) {
        error_flag();
    }

    Timer_Handle timer0 = timer0Init();
    if (timer0 == NULL) {
        error_flag();
    }

//    if (Timer_start(timer0) == Timer_STATUS_ERROR) {
//        error_flag();
//    }

    GPIO_setConfig(Board_LED_RED, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_LOW);

    int timeValue = 0;
    int start_timer0 = 0;
    uint32_t wheel0_prev_pos = 0;
    uint32_t wheel1_prev_pos = 0;
    uint32_t wheel2_prev_pos = 0;

    while (1) {
        if (receiveMsgFromEncoderQ(&timeValue, &start_timer0)) {
            //GPIO_toggle(Board_LED_RED);
            if (timeValue) {
                uint32_t wheel0_pos = read_encoder(&encoder_spi, Slave_Select0);
                uint32_t wheel0_speed = getSpeed(wheel0_prev_pos, wheel0_pos);

                uint32_t wheel1_pos = read_encoder(&encoder_spi, Slave_Select1);
                uint32_t wheel1_speed = getSpeed(wheel1_prev_pos, wheel1_pos);

                uint32_t wheel2_pos = read_encoder(&encoder_spi, Slave_Select2);
                uint32_t wheel2_speed = getSpeed(wheel2_prev_pos, wheel2_pos);

                // send the speed to Control
                status = sendWheels_SpeedMsgToControlQ(wheel0_speed, wheel1_speed, wheel2_speed);

                // send the speed to MQTT
                sendWheelsSpeedToMQTT(wheel0_speed, wheel1_speed, wheel2_speed);

//                if (wheel0_speed == 0 && wheel1_speed == 0 && wheel2_speed == 0) {
//                    sendStillToMQTT(1);
//                }
//                else {
//                    sendStillToMQTT(0);
//                }

                if (!status) {
                    error_flag();
                }

                wheel0_prev_pos = wheel0_pos;
                wheel1_prev_pos = wheel1_pos;
                wheel2_prev_pos = wheel2_pos;
                timeValue = 0;
            }
            else if (start_timer0) {
                if (Timer_start(timer0) == Timer_STATUS_ERROR) {
                    error_flag();
                }
                start_timer0 = 0;
            }
        }
        else {
            error_flag();
        }
    }


    //return (NULL);
}

int init_encoder(SPI_Handle* spiHandle, unsigned index) {

    // encoder 1
    GPIO_write(index, 0);
    int status = spi_init_encoder_mode(spiHandle);
    GPIO_write(index, 1);

    GPIO_write(index, 0);
    status = spi_clear_encoder(spiHandle);
    GPIO_write(index, 1);

    return status;
}

uint32_t read_encoder(SPI_Handle* spiHandle, unsigned index) {
    GPIO_write(index, 0);

    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
    int status = spi_read_encoder(spiHandle, &byte0, &byte1, &byte2, &byte3);
    uint32_t result = (byte3 << 8) + byte2;
    result = (result << 8) + byte1;
    result = (result << 8) + byte0;

    GPIO_write(index, 1);

    // get the count value
    return result;
}

uint32_t getSpeed(uint32_t prev_counts, uint32_t counts) {
    int64_t prev_temp = prev_counts;
    int64_t temp = counts;
    int64_t passed_pos = temp - prev_temp;
    if (passed_pos >= 0) {
        if (passed_pos > Max_Diff) {
            // opposite dir, underflow
            passed_pos = prev_temp + MAX_32BITS - temp;
        }
    }
    else {
        passed_pos = -passed_pos;
        if (passed_pos > Max_Diff) {
            // overflow
            passed_pos = MAX_32BITS - prev_temp + temp;
        }
    }

    uint32_t result = passed_pos;

    return result;
}
