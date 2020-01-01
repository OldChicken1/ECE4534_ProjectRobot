/*
 * control_thread.c
 *
 *  Created on: Sep 29, 2019
 *      Author: lizid
 */

#include "control_thread.h"

void *control_threadFxn(void *arg0) {
    UART_Handle uart;

    initialize(&uart);

    sendStartTimerMsgToEncoderQ();

    Wheel_Speed_Type wheel_speed;
    Wheel_Move_Type wheel0_move;
    Wheel_Move_Type wheel1_move;
    Wheel_Move_Type wheel2_move;

    init_wheel(&wheel0_move);
    init_wheel(&wheel1_move);
    init_wheel(&wheel2_move);

    int64_t wheel0_integral = 0;
    int64_t wheel1_integral = 0;
    int64_t wheel2_integral = 0;

    while (1) {
        if (receiveMsgFromControlQ(&wheel_speed, &wheel0_move, &wheel1_move, &wheel2_move)) {
            /*
            if (wheel0_move.updated) {

                uint8_t speed = wheel0_move.speed;
                move_wheel(uart, Controller1_addr, wheel0_move.direction, speed);
                //wheel0_integral = 0;
                wheel0_move.updated = 0;
            }
            else if (wheel1_move.updated) {
                //wheel1_integral = 0;
                wheel1_move.updated = 0;
            }
            else if (wheel2_move.updated) {
                //wheel2_integral = 0;
                wheel2_move.updated = 0;
            }*/
            if (wheel_speed.updated) {
                int64_t wheel0_ref_speed = wheel0_move.speed;
                int64_t wheel1_ref_speed = wheel1_move.speed;
                int64_t wheel2_ref_speed = wheel2_move.speed;
                int64_t wheel0_cur_speed = wheel_speed.speed1;
                int64_t wheel1_cur_speed = wheel_speed.speed2;
                int64_t wheel2_cur_speed = wheel_speed.speed3;
                int64_t wheel0_error = wheel0_ref_speed - wheel0_cur_speed;
                int64_t wheel1_error = wheel1_ref_speed - wheel1_cur_speed;
                int64_t wheel2_error = wheel2_ref_speed - wheel2_cur_speed;
                uint8_t wheel0_output = run_pi(Controller1_addr, wheel0_error, &wheel0_integral, Wheel0_Kp, Wheel0_Ki, Time_Interval);
                uint8_t wheel1_output = run_pi(Controller2_addr, wheel1_error, &wheel1_integral, Wheel1_Kp, Wheel1_Ki, Time_Interval);
                uint8_t wheel2_output = run_pi(Controller3_addr, wheel2_error, &wheel2_integral, Wheel2_Kp, Wheel2_Ki, Time_Interval);
                //UART_PRINT("wheel0_integral: %lld\n\r", wheel0_integral);
                //UART_PRINT("wheel1_integral: %lld\n\r", wheel1_integral);
                //UART_PRINT("wheel2_integral: %lld\n\r", wheel2_integral);
                if (wheel0_ref_speed == 0) {
                    wheel0_integral = 0;
                }
                else if (wheel1_ref_speed == 0) {
                    wheel1_integral = 0;
                }
                else if (wheel2_ref_speed == 0) {
                    wheel2_integral = 0;
                }
                move_wheel(uart, Controller1_addr, wheel0_move.direction, wheel0_output);
                move_wheel(uart, Controller2_addr, wheel1_move.direction, wheel1_output);
                move_wheel(uart, Controller3_addr, wheel2_move.direction, wheel2_output);
                wheel_speed.updated = 0;
            }
        }
        else {
            error_flag();
        }
    }
}

void initialize(UART_Handle* uart) {
    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;
    *uart = UART_open(Board_UART1, &uartParams);

    if (uart == NULL) {
        error_flag();
    }

    // sent baud rate
    sleep(2);
    uint8_t baudChar = 170;
    UART_write(*uart, &baudChar, Byte_Length);

}

void init_wheel(Wheel_Move_Type* wheel) {
    wheel->direction = Command_Forward;
    wheel->speed = 0;
    wheel->updated = 0;
}

void move_wheel(UART_Handle uart, uint8_t address, uint8_t direction, uint8_t speed) {
    if (speed > 127) {
        speed = 127;
    }
    uint8_t checksum = (address + direction + speed) & CHECKSUM_BITS;
    uint8_t data[] = {address, direction, speed, checksum};
    UART_write(uart, data, sizeof(data));
}

uint8_t run_pi(int index, int64_t error, int64_t* integral, double Kp, double Ki, double dt) {
    *integral += error * dt;
    int64_t output = Kp * error + *integral * Ki;
    return convertWheelSpeedToByte(index, output);
}

uint8_t convertWheelSpeedToByte(int index, double speed) {
    double temp;
    switch (index) {
    case Controller1_addr:
        temp = 0.4262 * speed + 1.5075;
        break;
    case Controller2_addr:
        temp = 0.426 * speed + 1.2302;
        break;
    case Controller3_addr:
        temp = 0.4301 * speed + 1.2529;
        break;
    default:
        break;
    }
    temp += 0.5;
    int temp2 = temp;
    uint8_t result;
    if (temp2 > 127) {
        result = 127;
    }
    else if (temp2 < 0) {
        result = 0;
    }
    else {
        result = temp2;
    }
    return result;
}

