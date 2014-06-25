// Project settings

// ------ External makefile settings ------
// Modify the makefile Makefile-ACUAS if
// - the device is not:                         ATmega16
// - the frequency of the controller is not:	1 MHz
// - the project directory is not:				ACUAS\Example
//
// Adjust the device:
//        MCU = atmega16
//              ^^^^^^^^
// Adjust the frequency (do not forget to modify the frequence in the controller, use AVRProg):
//        CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=1000000UL -O0 -funsigned-char -funsigned-bitfields 
//                                                       ^^^^^^^
// Adjust the directory name:
//        INCLUDES = -I"../Example" -I"../" -I"../LCD" -I"../UART" -I"./" 
//                         ^^^^^^^

// ------ ACUAS general macros:  ------
#include <ACUAS.h>

// ------ LCD specific settings: ------
// 1.) ACUAS LCD macros:
#include <ACUASlcd.h>

// 2.) Specify LCD hardware:
//#define ACUAS_LCD_TYPE_162HD44780	// Displaytech 162A (2 x 16 characters) or
#define ACUAS_LCD_TYPE_164HD44780 	// Displaytech 164A (4 x 16 characters)

// 3.) Set wrap mode
#define ACUAS_LCD_WRAP_LINES 0		// 0: no wrap, 1: wrap at end of visibile line

// ------ UART specific settings: ------
// 1.) Set baud rate (default is 1200):
#define UART_BAUD_RATE 4800
// 2.) ACUAS UART macros:
#include <ACUASuart.h>

// ------ softUART specific settings: ------
// 1.) Set baud rate (default is 1200, maximum is 4800):
 #define SOFTUART_BAUD_RATE 4800
#include <ACUASsoftuart.h>

// ------ Configure assert: ------
#define NDEBUG // if defined: assert disabled
#define __ASSERT_USE_STDERR      // If defined: display error message on LCD
#define ASSERT_WAIT_SECS 10      // Countdown in seconds before the device is resetted (by watchdog)
#define ASSERT_CONFIRM_BUTTON 2  // Button by which the countdown can be stopped. Second push immediately resets the device.
