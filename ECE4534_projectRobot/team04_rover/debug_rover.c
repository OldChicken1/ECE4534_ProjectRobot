/*
 * debug.c
 *
 *  Created on: Oct 7, 2019
 *      Author: lizid
 */

#include <debug_rover.h>

void gpioButtonFxn0(uint_least8_t index)
{
    sendButtonPressedMsgToDebugQ();
}

void *debug_threadFxn(void *arg0) {
    /*UART_Handle uart_usb;
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uart_usb = UART_open(Board_UART1, &uartParams);*/

    //if (uart_usb == NULL) {
    //    impl_error_flag();
    //}

    // char prompt[40] = {0};

    int error = 0;
    int button_pressed = 0;
    int timer_expired = 0;

    GPIO_setConfig(Board_Button_SW3, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);
    GPIO_setCallback(Board_Button_SW3, gpioButtonFxn0);
    GPIO_enableInt(Board_Button_SW3);

    Timer_Handle timer1 = timer1Init();

    /*
     * test0: motors stop
     * test1: motor 1 rotates
     * test2: motor 2 rotates, motor 1 stops
     * test3: motor 3 rotates, motor 2 stops
     * test4: all motors move forward
     * test5: all motors move backward
     * test6: motors with all different speeds, same direction
     * test7: move forward
     * test8: move left
     * test9: move right
     * test10: move back
     */
    test_state_Type test_state = test1;
    if (Timer_start(timer1) == Timer_STATUS_ERROR) {
        impl_error_flag();
    }

    while (1) {
        if (receiveMsgFromDebugQ(&error, &button_pressed, &timer_expired)) {
            if (error) {
                impl_error_flag();
            }
            else if (button_pressed && timer_expired) {
                // Execute state
                test_fxn(&test_state);

                timer_expired = 0;
                button_pressed = 0;
                if (Timer_start(timer1) == Timer_STATUS_ERROR) {
                    impl_error_flag();
                }

            } /*
            else if (wheel0.updated) {
                int speed1 = wheel0.speed;
                sprintf(prompt, "wheel 1: %d\r\n", speed1);
                //UART_write(uart_usb, prompt, strlen(prompt));
                wheel0.updated = 0;
            }
            else if (wheel1.updated) {
                int speed1 = wheel1.speed;
                sprintf(prompt, "wheel 2: %d\r\n", speed1);
                //UART_write(uart_usb, prompt, strlen(prompt));
                wheel1.updated = 0;
            }
            else if (wheel2.updated) {
                int speed1 = wheel2.speed;
                sprintf(prompt, "wheel 3: %d\r\n\r\n", speed1);
                //UART_write(uart_usb, prompt, strlen(prompt));
                wheel2.updated = 0;
            }
            else {
            }*/
        }
        else {
            impl_error_flag();
        }
    }
}

void test_fxn(test_state_Type* test_state) {
    switch (*test_state) {
    case test0:
        sendWheel0ForwardMsgToControlQ(0);
        sendWheel1ForwardMsgToControlQ(0);
        sendWheel2ForwardMsgToControlQ(0);
        *test_state = test1;
        break;
    case test1:
        sendWheel0ForwardMsgToControlQ(30);
        *test_state = test2;
        break;
    case test2:
        sendWheel0ForwardMsgToControlQ(0);
        sendWheel1ForwardMsgToControlQ(30);
        *test_state = test3;
        break;
    case test3:
        sendWheel1ForwardMsgToControlQ(0);
        sendWheel2ForwardMsgToControlQ(30);
        *test_state = test4;
        break;
    case test4:
        sendWheel0ForwardMsgToControlQ(30);
        sendWheel1ForwardMsgToControlQ(30);
        sendWheel2ForwardMsgToControlQ(30);
        *test_state = test5;
        break;
    case test5:
        sendWheel0BackwardMsgToControlQ(30);
        sendWheel1BackwardMsgToControlQ(30);
        sendWheel2BackwardMsgToControlQ(30);
        *test_state = test6;
        break;
    case test6:
        sendWheel0ForwardMsgToControlQ(30);
        sendWheel1ForwardMsgToControlQ(50);
        sendWheel2ForwardMsgToControlQ(70);
        *test_state = test7;
        break;
    case test7:
        // move forward
        sendWheel0ForwardMsgToControlQ(0);
        sendWheel1BackwardMsgToControlQ(30);
        sendWheel2ForwardMsgToControlQ(30);
        *test_state = test8;
        break;
    case test8:
        // move left
        sendWheel0ForwardMsgToControlQ(30);
        sendWheel1BackwardMsgToControlQ(15);
        sendWheel2BackwardMsgToControlQ(15);
        *test_state = test9;
        break;
    case test9:
        // move right
        sendWheel0BackwardMsgToControlQ(30);
        sendWheel1ForwardMsgToControlQ(15);
        sendWheel2ForwardMsgToControlQ(15);
        *test_state = test10;
        break;
    case test10:
        // move back
        sendWheel0ForwardMsgToControlQ(0);
        sendWheel1ForwardMsgToControlQ(30);
        sendWheel2BackwardMsgToControlQ(30);
        *test_state = test0;
        break;
    default:
        break;
    }
}

void impl_error_flag() {
   int retc = 0;

   HwiP_disable();
   vTaskSuspendAll();
   pthread_exit((void *)retc);
}
