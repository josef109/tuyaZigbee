/********************************************************************************************************
 * @file    board_8258_evk.h
 *
 * @brief   This is the header file for board_8258_evk
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


// BUTTON
#define BUTTON1               GPIO_PD2
#define PD2_FUNC			  AS_GPIO
#define PD2_OUTPUT_ENABLE	  0
#define PD2_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD2	  PM_PIN_PULLUP_10K

// #define BUTTON2               GPIO_PD2
// #define PD2_FUNC			  AS_GPIO
// #define PD2_OUTPUT_ENABLE	  0
// #define PD2_INPUT_ENABLE	  1
// #define	PULL_WAKEUP_SRC_PD2	  PM_PIN_PULLUP_10K

// INPUTS
#define BUTTON_UP             GPIO_PD4
#define PD4_FUNC			  AS_GPIO
#define PD4_OUTPUT_ENABLE	  0
#define PD4_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PD4	  PM_PIN_PULLUP_10K

#define BUTTON_DOWN           GPIO_PC3
#define PC3_FUNC			  AS_GPIO
#define PC3_OUTPUT_ENABLE	  0
#define PC3_INPUT_ENABLE	  1
#define	PULL_WAKEUP_SRC_PC3	  PM_PIN_PULLUP_10K


// LED

#define LED_R						GPIO_PA0

#define PA0_FUNC					AS_GPIO
#define PA0_OUTPUT_ENABLE			1
#define PA0_INPUT_ENABLE			0

#define LED_PERMIT					LED_R

// RELAY
#define RELAY_UP					GPIO_PC0
#define RELAY_DOWN					GPIO_PC2

#define PC0_FUNC					AS_GPIO
#define PC0_OUTPUT_ENABLE			1
#define PC0_INPUT_ENABLE			0

#define PC2_FUNC					AS_GPIO
#define PC2_OUTPUT_ENABLE			1
#define PC2_INPUT_ENABLE			0



// UART
#if ZBHCI_UART
	#error please configurate uart PIN!!!!!!
#endif

// DEBUG
#if UART_PRINTF_MODE
	#define	DEBUG_INFO_TX_PIN	    GPIO_PC7//print
#endif


enum{
	VK_SW1 = 0x01,
	VK_SW2 = 0x02,
	VK_SW3 = 0x02,
};

#define	KB_MAP_NORMAL	{\
		{VK_SW1,}, \
		{VK_SW2,}, \
		{VK_SW3,}, }

#define	KB_MAP_NUM		KB_MAP_NORMAL
#define	KB_MAP_FN		KB_MAP_NORMAL

#define KB_DRIVE_PINS  {NULL }
#define KB_SCAN_PINS   {BUTTON1, BUTTON_DOWN, BUTTON_UP}


/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
