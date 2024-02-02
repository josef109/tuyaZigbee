/********************************************************************************************************
 * @file    zcl_levelCb.c
 *
 * @brief   This is the source file for zcl_levelCb
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

// Fahrzeit auf       57s = 100%   0.1s = 0,175438596491%    = 44,9122807017 (1/256)    
// Fahrzeit zu        57s
// Wartezeit bei Richtungswechsel  1.5s
// Verstellzeit Lamellen   1.6s


#if (__PROJECT_TL_ROLLER_SHUTTER__)
/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "tuyaShutter.h"
#include "tuyaShutterCtrl.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */
#define ZCL_LEVEL_CHANGE_INTERVAL		100

enum shutterState {
	IDLE,
    OPEN_TILT,
    CLOSE_TILT,
	UP,
	DOWN,
    BREAK_UP,
	BREAK_DOWN,
};

/**********************************************************************
 * TYPEDEFS
 */
typedef struct
{
	u16 destinationLiftPerc256;
	u16 currentLiftPerc256;
	u16 destinationTiltPerc256;
	u16 currentTiltPerc256;
	u16 stepUpTiltPerc256;
	u16 stepUpLiftPerc256;
	u16 stepDownTiltPerc256;
	u16 stepDownLiftPerc256;
	u16 remainingTime;
	u8 state;
} zcl_CoveringInfo_t;

/**********************************************************************
 * LOCAL VARIABLES
 */
static zcl_CoveringInfo_t coveringInfo = {
	.destinationLiftPerc256 = 0,
	.currentLiftPerc256 = 0,
	.destinationTiltPerc256 = 0,
	.currentTiltPerc256 = 0,
};

static ev_timer_event_t *levelTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

void pin_on(u32 pin){
	drv_gpio_write(pin, 1);
}

void pin_off(u32 pin){
	drv_gpio_write(pin, 0);
}

void powerOff(void)
{
	pin_off(RELAY_UP);
	pin_off(RELAY_DOWN);
}

void moveUp(void)
{
	pin_off(RELAY_DOWN);
	pin_on(RELAY_UP);
}

void moveDown(void)
{
	pin_off(RELAY_UP);
	pin_on(RELAY_DOWN);
}

/*********************************************************************
 * @fn      tuyaShutter_coverInit
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void tuyaShutter_coverInit(void)
{
	zcl_WindowCoveringAttr_t *pLevel = zcl_WindowCoveringGet();

	coveringInfo.currentLiftPerc256 = (u16)(pLevel->CurrentPositionLiftPercentage) << 8;
	coveringInfo.state = IDLE;
	powerOff();
}


/*********************************************************************
 * @fn      tuyaShutter_levelTimerEvtCb
 *
 * @brief   timer event to process the level command
 *
 * @param	arg
 *
 * @return  0: timer continue on; -1: timer will be canceled
 */
static s32 tuyaShutter_levelTimerEvtCb(void *arg)
{
	zcl_WindowCoveringAttr_t *pLevel = zcl_WindowCoveringGet();

	switch (coveringInfo.state)
	{
	case IDLE:
		if (coveringInfo.currentLiftPerc256 > coveringInfo.destinationLiftPerc256)
		{
			coveringInfo.state = UP;
			moveUp();
		}
		else if (coveringInfo.currentLiftPerc256 < coveringInfo.destinationLiftPerc256)
		{
			coveringInfo.state = DOWN;
			moveDown();
		}
		// lift up or down
		break;
	case OPEN_TILT:
		break;
	case CLOSE_TILT:
		break;
	case UP:
		if (coveringInfo.currentTiltPerc256 >= (100 << 8))
		{
			if (coveringInfo.currentLiftPerc256 >= (100 << 8)){				powerOff();
				coveringInfo.state = BREAK_DOWN;}
			else
				coveringInfo.currentLiftPerc256 += coveringInfo.stepUpLiftPerc256;
		}
		else
		{
			coveringInfo.currentTiltPerc256 += coveringInfo.stepUpTiltPerc256;
		}
		break;
	case DOWN:
		break;
	case BREAK_UP:
		break;
	case BREAK_DOWN:
		break;
	}

	if (coveringInfo.remainingTime)
	{
		return 0;
	}
	else
	{
		levelTimerEvt = NULL;
		return -1;
	}
}

/*********************************************************************
 * @fn      tuyaShutter_LevelTimerStop
 *
 * @brief   force to stop the level timer
 *
 * @param
 *
 * @return
 */
static void tuyaShutter_LevelTimerStop(void)
{
	if(levelTimerEvt){
		TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	}
}

/*********************************************************************
 * @fn      tuyaShutter_moveToLiftPercentageProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void tuyaShutter_moveToLiftPercentageProcess(u8 cmdId, zcl_goToLiftPer_t *cmd)
{
	zcl_WindowCoveringAttr_t *pLevel = zcl_WindowCoveringGet();

	coveringInfo.currentLiftPerc256 = (u16)(pLevel->CurrentPositionLiftPercentage) << 8;
	coveringInfo.currentTiltPerc256= (u16)(pLevel->CurrentPositionTiltPercentage) << 8;

	//coveringInfo.stepLevel256 = ((s32)(cmd->level - pLevel->curLevel)) << 8;
	//coveringInfo.stepLevel256 /= (s32)pLevel->remainingTime;


}


/*********************************************************************
 * @fn      tuyaShutter_windowCoveringCb
 *
 * @brief   Handler for ZCL LEVEL command. This function will set LEVEL attribute first.
 *
 * @param	pAddrInfo
 * @param   cmd - level cluster command id
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t tuyaShutter_windowCoveringCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == TUYA_SHUTTER_ENDPOINT){
		switch(cmdId){
			case ZCL_CMD_UP_OPEN:
				//tuyaShutter_moveToLiftPercentageProcess(cmdId, (moveToLvl_t *)cmdPayload);
				break;
			case ZCL_CMD_DOWN_CLOSE:
				//tuyaShutter_moveProcess(cmdId, (move_t *)cmdPayload);
				break;
			case ZCL_CMD_STOP:
				//tuyaShutter_stopProcess(cmdId, (step_t *)cmdPayload);
				break;
			case ZCL_CMD_GO_TO_LIFT_VALUE:
				//tuyaShutter_moveToLiftPercentageProcess(cmdId, (moveToLvl_t *)cmdPayload);
				break;
			case ZCL_CMD_GO_TO_LIFT_PERCENTAGE:
				tuyaShutter_moveToLiftPercentageProcess(cmdId, (zcl_goToLiftPer_t *)cmdPayload);

				break;
			case ZCL_CMD_GO_TO_TILT_VALUE:
				
				break;
			case ZCL_CMD_GO_TO_TILT_PERCENTAGE:
				
				break;
			default:
				break;
		}
	}

	return ZCL_STA_SUCCESS;
}



#endif  /* __PROJECT_TL_ROLLER_SHUTTER__ */
