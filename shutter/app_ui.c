/********************************************************************************************************
 * @file    app_ui.c
 *
 * @brief   This is the source file for app_ui
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

#if (__PROJECT_TL_ROLLER_SHUTTER__)

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "tuyaShutter.h"
#include "app_ui.h"
#include "gp.h"
/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
void led_on(u32 pin){
	drv_gpio_write(pin, LED_ON);
}

void led_off(u32 pin){
	drv_gpio_write(pin, LED_OFF);
}

void led_init(void){
	//led_off(LED_POWER);
	led_off(LED_PERMIT);
}

void localPermitJoinState(void){
	// static bool assocPermit = 0;
	// if(assocPermit != zb_getMacAssocPermit()){
	// 	assocPermit = zb_getMacAssocPermit();
	// 	if(assocPermit){
	// 		led_on(LED_PERMIT);
	// 	}else{
	// 		led_off(LED_PERMIT);
	// 	}
	// }
}

void light_on(void)
{
	led_on(LED_PERMIT);
}

void light_off(void)
{
	led_off(LED_PERMIT);
}

s32 zclLightTimerCb(void *arg)
{
	u32 interval = 0;

	if(gShutterCtx.sta == gShutterCtx.oriSta){
		gShutterCtx.times--;
		if(gShutterCtx.times <= 0){
			gShutterCtx.timerLedEvt = NULL;
			return -1;
		}
	}

	gShutterCtx.sta = !gShutterCtx.sta;
	if(gShutterCtx.sta){
		light_on();
		interval = gShutterCtx.ledOnTime;
	}else{
		light_off();
		interval = gShutterCtx.ledOffTime;
	}

	return interval;
}

void light_blink_start(u8 times, u16 ledOnTime, u16 ledOffTime)
{
	u32 interval = 0;
	gShutterCtx.times = times;

	if(!gShutterCtx.timerLedEvt){
		if(gShutterCtx.oriSta){
			light_off();
			gShutterCtx.sta = 0;
			interval = ledOffTime;
		}else{
			light_on();
			gShutterCtx.sta = 1;
			interval = ledOnTime;
		}
		gShutterCtx.ledOnTime = ledOnTime;
		gShutterCtx.ledOffTime = ledOffTime;

		gShutterCtx.timerLedEvt = TL_ZB_TIMER_SCHEDULE(zclLightTimerCb, NULL, interval);
	}
}

void light_blink_stop(void)
{
	if(gShutterCtx.timerLedEvt){
		TL_ZB_TIMER_CANCEL(&gShutterCtx.timerLedEvt);

		gShutterCtx.times = 0;
		if(gShutterCtx.oriSta){
			light_on();
		}else{
			light_off();
		}
	}
}

void buttonKeepPressed(u8 btNum){

	printf("Button keep pressed %d\n",btNum);

	if(btNum == VK_SW1){
		gShutterCtx.state = APP_FACTORY_NEW_DOING;
		zb_factoryReset();
	}else if(btNum == VK_SW2){
		;
	}
}

void buttonShortPressed(u8 btNum){

	printf("buttonShortPressed %d\n",btNum);

	if(btNum == VK_SW1){
		if(zb_isDeviceJoinedNwk()){
			gShutterCtx.sta = !gShutterCtx.sta;
			// if(gShutterCtx.sta){
			// 	tuyaShutter_onoff(ZCL_ONOFF_STATUS_ON);
			// }else{
			// 	tuyaShutter_onoff(ZCL_ONOFF_STATUS_OFF);
			// }
		}
	}
	else if (btNum == VK_SW2)
	{
		if (!moveFlag())
		{
			if (g_zcl_WindowCoveringAttrs.ReverseFlags & 2)
				tuyaShutter_openProcess();
			else 
				tuyaShutter_closeProcess();
		}
		else
			tuyaShutter_stopProcess();

		/* toggle local permit Joining */
		// u8 duration = zb_getMacAssocPermit() ? 0 : 180;
		// zb_nlmePermitJoiningRequest(duration);

		// gpsCommissionModeInvork();
	}
	else if (btNum == VK_SW3)
	{
		if (!moveFlag())
		{
			if (g_zcl_WindowCoveringAttrs.ReverseFlags & 2)
				tuyaShutter_closeProcess();
			else
				tuyaShutter_openProcess();
		}
		else
			tuyaShutter_stopProcess();
		/* toggle local permit Joining */
		// u8 duration = zb_getMacAssocPermit() ? 0 : 180;
		// zb_nlmePermitJoiningRequest(duration);

		// gpsCommissionModeInvork();
	}
}

void keyScan_keyPressedCB(kb_data_t *kbEvt)
{
	//	u8 toNormal = 0;
	u8 keyCode = kbEvt->keycode[0];
	//	static u8 lastKeyCode = 0xff;

	buttonShortPressed(keyCode);

	if (keyCode == VK_SW1)
	{
		gShutterCtx.keyPressedTime = clock_time();
		gShutterCtx.state = APP_FACTORY_NEW_SET_CHECK;
	}
	if ((keyCode == VK_SW2) || (keyCode == VK_SW3))
		gShutterCtx.keyPressedTimeUpDown = clock_time();
}

void keyScan_keyReleasedCB(u8 keyCode)
{
	printf("Button keep pressed %d\n", keyCode);
	if (keyCode == VK_SW1)
	{
		gShutterCtx.state = APP_STATE_NORMAL;
	}
	if((keyCode == VK_SW2) || (keyCode == VK_SW3))
	{
		if(clock_time_exceed(gShutterCtx.keyPressedTimeUpDown, 2*600*1000))
			;
		else if(moveFlag())
			tuyaShutter_stopProcess();

	}
}

//volatile u8 T_keyPressedNum = 0;
void app_key_handler(void){
	static u8 valid_keyCode = 0xff;

	if(gShutterCtx.state == APP_FACTORY_NEW_SET_CHECK){
		if(clock_time_exceed(gShutterCtx.keyPressedTime, 5*1000*1000)){
			buttonKeepPressed(VK_SW1);
		}
	}

	if(kb_scan_key(0 , 1)){
//		T_keyPressedNum++;
		if(kb_event.cnt){
			keyScan_keyPressedCB(&kb_event);
			if(kb_event.cnt == 1){
				valid_keyCode = kb_event.keycode[0];
			}
		}else{
			keyScan_keyReleasedCB(valid_keyCode);
			valid_keyCode = 0xff;
		}
	}
}

#endif  /* __PROJECT_TL_ROLLER_SHUTTER__ */
