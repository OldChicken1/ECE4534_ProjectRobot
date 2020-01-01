/*
 *  ======== Board.h ========
 *  Configured TI-Drivers module declarations
 *
 *  DO NOT EDIT - This file is generated for the CC3220SF_LAUNCHXL
 *  by the SysConfig tool.
 */
#ifndef Board_h
#define Board_h

#define Board_SYSCONFIG_PREVIEW

#define Board_CC3220SF_LAUNCHXL

/* Temporary define for backwards compatibility!!! */
#define __CC3220SF_LAUNCHXL_BOARD_H__

#ifndef DeviceFamily_CC3220
#define DeviceFamily_CC3220
#endif

#include <stdint.h>

/* support C++ sources */
#ifdef __cplusplus
extern "C" {
#endif


/*
 *  ======== Crypto ========
 */

#define Board_Crypto0               0

/*
 *  ======== GPIO ========
 */

#define Board_GPIO0                 0
#define Board_Button_SW3            1
#define Board_GPIO1                 2
#define Board_GPIO2                 3
#define Board_GPIO3                 4
#define Board_GPIO4                 5
#define Board_GPIO5                 6
#define Board_GPIO6                 7
#define Board_GPIO7                 8
#define Slave_Select0               9
#define Slave_Select1               10
#define Slave_Select2               11
#define Board_LED_RED               12

/* LEDs are active high */
#define Board_GPIO_LED_ON  (1)
#define Board_GPIO_LED_OFF (0)

#define Board_LED_ON  (Board_GPIO_LED_ON)
#define Board_LED_OFF (Board_GPIO_LED_OFF)


/*
 *  ======== SPI ========
 */

#define Board_NWP_SPI               0
#define Board_SPI0                  1


/*
 *  ======== Timer ========
 */

#define Board_TIMER0                0
#define Board_TIMER1                1


/*
 *  ======== UART ========
 */

#define Board_UART1                 0
#define Board_UART0                 1


/*
 *  ======== Watchdog ========
 */

#define Board_WATCHDOG0             0


/*
 *  ======== Board_init ========
 *  Perform all required TI-Drivers initialization
 *
 *  This function should be called once at a point before any use of
 *  TI-Drivers.
 */
extern void Board_init(void);

/*
 *  ======== Board_initGeneral ========
 *  (deprecated)
 *
 *  Board_initGeneral() is defined purely for backward compatibility.
 *
 *  All new code should use Board_init() to do any required TI-Drivers
 *  initialization _and_ use <Driver>_init() for only where specific drivers
 *  are explicitly referenced by the application.  <Driver>_init() functions
 *  are idempotent.
 */
#define Board_initGeneral Board_init

#ifdef __cplusplus
}
#endif

#endif /* include guard */
