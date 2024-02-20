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
#define ZCL_LEVEL_CHANGE_INTERVAL 100

enum shutterState
{
	IDLE,
	OPEN_TILT,
	CLOSE_TILT,
	UP,
	DOWN,
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
	s16 stepUpTiltPerc256;
	s16 stepUpLiftPerc256;
	s16 stepDownTiltPerc256;
	s16 stepDownLiftPerc256;
	u16 lockTimerUp;
	u16 lockTimerDown;
	u16 endTimer;
	u16 remainingTime;
	u8 state;
} zcl_CoveringInfo_t;

/**********************************************************************
 * LOCAL VARIABLES
 */
static zcl_CoveringInfo_t coveringInfo;

static ev_timer_event_t *levelTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */

static void tuyaShutter_LevelTimerStop(void);

void pin_on(u32 pin)
{
	drv_gpio_write(pin, 1);
}

void pin_off(u32 pin)
{
	drv_gpio_write(pin, 0);
}

void powerOff(void)
{
	pin_off(RELAY_UP);
	pin_off(RELAY_DOWN);
}

void moveUp(void)
{
	if (g_zcl_nv_WindowCovering.ReverseFlags & 1)
	{
		pin_off(RELAY_UP);
		pin_on(RELAY_DOWN);
	}
	else
	{
		pin_off(RELAY_DOWN);
		pin_on(RELAY_UP);
	}
}

void moveDown(void)
{
	if (g_zcl_nv_WindowCovering.ReverseFlags & 1)
	{
		pin_off(RELAY_DOWN);
		pin_on(RELAY_UP);
	}
	else
	{
		pin_off(RELAY_UP);
		pin_on(RELAY_DOWN);
	}
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
	coveringInfo.currentTiltPerc256 = (u16)(pLevel->CurrentPositionTiltPercentage) << 8;
	coveringInfo.destinationLiftPerc256 = coveringInfo.currentLiftPerc256;
	coveringInfo.destinationTiltPerc256 = coveringInfo.currentTiltPerc256;

	coveringInfo.state = IDLE;

	if(g_zcl_nv_WindowCovering.TiltMoveTime == 0) {
		coveringInfo.stepDownTiltPerc256 = 255;
		coveringInfo.stepUpTiltPerc256 = 255;
	}
	else {
		coveringInfo.stepDownTiltPerc256 = (u16)(100 << 8) / g_zcl_nv_WindowCovering.TiltMoveTime; // 0 - 100     100% / 50
		coveringInfo.stepUpTiltPerc256 = -(s16)((u16)(100 << 8) / g_zcl_nv_WindowCovering.TiltMoveTime);
	}
	
	if(g_zcl_nv_WindowCovering.LiftTimeDown == 0)
		coveringInfo.stepDownLiftPerc256 = 255;
	else
		coveringInfo.stepDownLiftPerc256 = (u16)(100 << 8) / g_zcl_nv_WindowCovering.LiftTimeDown; // 57s

	if(g_zcl_nv_WindowCovering.LiftTimeUp == 0)
		coveringInfo.stepUpLiftPerc256 = 255;
	else
		coveringInfo.stepUpLiftPerc256 = -(s16)((u16)(100 << 8) / g_zcl_nv_WindowCovering.LiftTimeUp);

	powerOff();
}

static inline bool checkmove(u16 cur, u16 des, s16 step)
{
	s16 dif = des - cur;
	if (step > 0 && dif > 0)
		return (dif >= step);
	else if (step < 0 && dif < 0)
		return (dif <= step);
	return FALSE;
}

s32 doIdle(void)
{
	powerOff();
	if (coveringInfo.lockTimerDown > 0)
		--coveringInfo.lockTimerDown;
	else if (coveringInfo.lockTimerUp > 0)
		--coveringInfo.lockTimerUp;
	else 
	{
	    printf("doIdle lockTimerDown: %d lockTimerUp: %d\n", coveringInfo.lockTimerDown, coveringInfo.lockTimerUp);
		levelTimerEvt = NULL;
		return -1;
	}
	printf("doIdle lockTimerDown: %d lockTimerUp: %d\n", coveringInfo.lockTimerDown, coveringInfo.lockTimerUp);
	return 0;
	
}

void doOpenTilt(void)
{
	coveringInfo.currentTiltPerc256 += coveringInfo.stepUpTiltPerc256;
	if (coveringInfo.currentTiltPerc256 < -coveringInfo.stepUpTiltPerc256)
		coveringInfo.currentTiltPerc256 = 0;
	moveUp();
	printf("doOpenTilt currentTiltPerc256: %d \n", coveringInfo.currentTiltPerc256);
}

void doCloseTilt(void)
{

	coveringInfo.currentTiltPerc256 += coveringInfo.stepDownTiltPerc256;
	if ((coveringInfo.currentTiltPerc256 + coveringInfo.stepDownTiltPerc256) > (100 << 8))
		coveringInfo.currentTiltPerc256 = 100 << 8;
	moveDown();
	printf("doCloseTilt currentTiltPerc256: %d \n", coveringInfo.currentTiltPerc256);
}

void doUp(void)
{
	if (coveringInfo.currentLiftPerc256 > 0)
	{
		coveringInfo.currentLiftPerc256 += coveringInfo.stepUpLiftPerc256;
		if (coveringInfo.currentLiftPerc256 < -coveringInfo.stepUpLiftPerc256)
			coveringInfo.currentLiftPerc256 = 0;
		if (coveringInfo.destinationLiftPerc256 == 0)
		{
			if (coveringInfo.destinationTiltPerc256 == 0)
				coveringInfo.endTimer = g_zcl_nv_WindowCovering.LiftTimeUp;
			else
				coveringInfo.endTimer = 20;
		}
		else
			coveringInfo.endTimer = 0;
	}
	else if(coveringInfo.endTimer > 0)
	{
		if ((coveringInfo.destinationTiltPerc256 > 0) && (coveringInfo.endTimer > 20))
			coveringInfo.endTimer = 20;
		--coveringInfo.endTimer;
	}
	moveUp();
	printf("doUp endtimer: %d currentLiftPerc256: %d\n", coveringInfo.endTimer, coveringInfo.currentLiftPerc256);
}

void doDown(void)
{
	if (coveringInfo.currentLiftPerc256 < (100 << 8))
	{
		coveringInfo.currentLiftPerc256 += coveringInfo.stepDownLiftPerc256;
		if ((coveringInfo.currentLiftPerc256 + coveringInfo.stepDownLiftPerc256) >= (100 << 8))
		{
			coveringInfo.currentLiftPerc256 = 100 << 8;
			if (coveringInfo.destinationTiltPerc256 >= (100 << 8))
				coveringInfo.endTimer = g_zcl_nv_WindowCovering.LiftTimeDown;
			else
				coveringInfo.endTimer = 20;
		}
		else
			coveringInfo.endTimer = 0;
	}
	else if(coveringInfo.endTimer > 0)	
	{		
		if ((coveringInfo.destinationTiltPerc256 < (100 << 8)) && (coveringInfo.endTimer > 20))
			coveringInfo.endTimer = 20;
		--coveringInfo.endTimer;
	}
	moveDown();
	printf("doDown endtimer: %d currentLiftPerc256: %d\n", coveringInfo.endTimer, coveringInfo.currentLiftPerc256);
}


bool moveFlag(void)
{
	return(coveringInfo.state != IDLE);
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
	s32 ret = 0;
	switch (coveringInfo.state)
	{
	case IDLE:
		if ((coveringInfo.lockTimerUp == 0) 
		&& checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256)) 
		{
			if(checkmove(coveringInfo.currentTiltPerc256, 0, coveringInfo.stepUpTiltPerc256))
			{
				coveringInfo.state = OPEN_TILT;
				doOpenTilt();
			}
			else
			{
				coveringInfo.state = UP;
				doUp();
			}
		}
		else if ((coveringInfo.lockTimerUp == 0) 
		&& checkmove(coveringInfo.currentTiltPerc256, coveringInfo.destinationTiltPerc256, coveringInfo.stepUpTiltPerc256))
		{
			coveringInfo.state = OPEN_TILT;
			doOpenTilt();
		}
		else if ((coveringInfo.lockTimerDown == 0) 
		&& checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256)) 
		{
			if(checkmove(coveringInfo.currentTiltPerc256, 100 << 8, coveringInfo.stepDownTiltPerc256))
			{
				coveringInfo.state = CLOSE_TILT;
				doCloseTilt();
			}
			else
			{
				coveringInfo.state = DOWN;
				doDown();
			}
		}
		else if ((coveringInfo.lockTimerDown == 0)
		&& checkmove(coveringInfo.currentTiltPerc256, coveringInfo.destinationTiltPerc256, coveringInfo.stepDownTiltPerc256))
		{
			coveringInfo.state = CLOSE_TILT;
			doCloseTilt();
		}
		else
			ret = doIdle();
		break;
	case OPEN_TILT:
		if ((coveringInfo.currentTiltPerc256 == 0) 
		&& (checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256)
		|| coveringInfo.destinationLiftPerc256 == 0))
		{
			coveringInfo.endTimer = g_zcl_nv_WindowCovering.LiftTimeUp;
			coveringInfo.state = UP;
			doUp();
		}
		else if (checkmove(coveringInfo.currentLiftPerc256 , coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256) 
		|| (!checkmove(coveringInfo.currentLiftPerc256 , coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256) 
		&& !checkmove(coveringInfo.currentTiltPerc256 , coveringInfo.destinationTiltPerc256, coveringInfo.stepUpTiltPerc256)))
		{
			coveringInfo.state = IDLE;
			coveringInfo.lockTimerDown = g_zcl_nv_WindowCovering.ReverseWaitTime;
			ret = doIdle();
		}
		else
			doOpenTilt();
		break;
	case CLOSE_TILT:
		if ((coveringInfo.currentTiltPerc256 == (100 << 8)) 
		&& (checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256)
		|| coveringInfo.destinationLiftPerc256 == (100 << 8)))
		{
			coveringInfo.endTimer = g_zcl_nv_WindowCovering.LiftTimeDown;
			coveringInfo.state = DOWN;
			doDown();
		}
		else if (checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256)
		|| (!checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256)
		&& !checkmove(coveringInfo.currentTiltPerc256, coveringInfo.destinationTiltPerc256, coveringInfo.stepDownTiltPerc256)))
		{
			coveringInfo.state = IDLE;
			coveringInfo.lockTimerUp = g_zcl_nv_WindowCovering.ReverseWaitTime;
			ret = doIdle();
		}
		else
			doCloseTilt();
		break;
	case UP:
		if (checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256)
		|| (!checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256) 
		&& coveringInfo.endTimer == 0))
		{
			coveringInfo.state = IDLE;
			coveringInfo.lockTimerDown = g_zcl_nv_WindowCovering.ReverseWaitTime;
			ret = doIdle();
		}
		else
			doUp();
		break;
	case DOWN:
		if (checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepUpLiftPerc256)
		|| (!checkmove(coveringInfo.currentLiftPerc256, coveringInfo.destinationLiftPerc256, coveringInfo.stepDownLiftPerc256)
		&& (coveringInfo.endTimer == 0)))
		{
			coveringInfo.state = IDLE;
			coveringInfo.lockTimerUp = g_zcl_nv_WindowCovering.ReverseWaitTime;
			ret = doIdle();
		}
		else
			doDown();
		break;
	}

	zcl_WindowCoveringAttr_t *pLevel = zcl_WindowCoveringGet();
	pLevel->CurrentPositionLiftPercentage = coveringInfo.currentLiftPerc256 >> 8;
	pLevel->CurrentPositionTiltPercentage = coveringInfo.currentTiltPerc256 >> 8;
	return ret;
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
	if (levelTimerEvt)
	{
		TL_ZB_TIMER_CANCEL(&levelTimerEvt);
		levelTimerEvt = NULL;
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
	coveringInfo.destinationLiftPerc256 = (u16)(cmd->perLiftValue) << 8;
	if (levelTimerEvt == NULL)
	{
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	}
	printf("Move Lift to %d\n", cmd->perLiftValue);

	// else if (!levelTimerEvt->isRunning)
	// {
	// 	TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	// 	levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	// }
}

/*********************************************************************
 * @fn      tuyaShutter_moveToTiltPercentageProcess
 *
 * @brief
 *
 * @param	cmdId
 * @param	cmd
 *
 * @return	None
 */
static void tuyaShutter_moveToTiltPercentageProcess(u8 cmdId, zcl_goToTiltPer_t *cmd)
{
	//	zcl_WindowCoveringAttr_t *pLevel = zcl_WindowCoveringGet();

	//	coveringInfo.currentLiftPerc256 = (u16)(pLevel->CurrentPositionLiftPercentage) << 8;
	//	coveringInfo.currentTiltPerc256 = (u16)(pLevel->CurrentPositionTiltPercentage) << 8;

	coveringInfo.destinationTiltPerc256 = (u16)(cmd->perTiltValue) << 8;
	// coveringInfo.stepLevel256 = ((s32)(cmd->level - pLevel->curLevel)) << 8;
	// coveringInfo.stepLevel256 /= (s32)pLevel->remainingTime;
	if (levelTimerEvt == NULL)
	{
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	}
	printf("Move Tilt to %d\n", cmd->perTiltValue);
	// else if (!levelTimerEvt->isRunning)
	// {
	// 	TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	// 	levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	// }
}

void tuyaShutter_openProcess(void)
{
	coveringInfo.destinationTiltPerc256 = 0;
	coveringInfo.destinationLiftPerc256 = 0;
	if (levelTimerEvt == NULL)
	{
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	}
	printf("Open\n");
	// else if (!levelTimerEvt->isRunning)
	// {
	// 	TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	// 	levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	// }
}

void tuyaShutter_closeProcess(void)
{
	coveringInfo.destinationTiltPerc256 = 100 << 8;
	coveringInfo.destinationLiftPerc256 = 100 << 8;
	if (levelTimerEvt == NULL)
	{
		levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	}
	printf("close\n");
	// else if (!levelTimerEvt->isRunning)
	// {
	// 	TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	// 	levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	// }
}

void tuyaShutter_stopProcess(void)
{
	coveringInfo.destinationTiltPerc256 = coveringInfo.currentTiltPerc256;
	coveringInfo.destinationLiftPerc256 = coveringInfo.currentLiftPerc256;
	coveringInfo.endTimer = 0;
	printf("stop\n");
	// else if (!levelTimerEvt->isRunning)
	// {
	// 	TL_ZB_TIMER_CANCEL(&levelTimerEvt);
	// 	levelTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_levelTimerEvtCb, NULL, ZCL_LEVEL_CHANGE_INTERVAL);
	// }
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
	u8 status = ZCL_STA_FAILURE;
	if (pAddrInfo->dstEp == TUYA_SHUTTER_ENDPOINT)
	{
		status = ZCL_STA_SUCCESS;
		switch (cmdId)
		{
		case ZCL_CMD_UP_OPEN:
			tuyaShutter_openProcess();
			break;
		case ZCL_CMD_DOWN_CLOSE:
			tuyaShutter_closeProcess();
			break;
		case ZCL_CMD_STOP:
			tuyaShutter_stopProcess();
			break;
		case ZCL_CMD_GO_TO_LIFT_VALUE:
			// tuyaShutter_moveToLiftValue(cmdId, (zcl_goToLiftPer_t *)cmdPayload);
			break;
		case ZCL_CMD_GO_TO_LIFT_PERCENTAGE:
			tuyaShutter_moveToLiftPercentageProcess(cmdId, (zcl_goToLiftPer_t *)cmdPayload);
			break;
		case ZCL_CMD_GO_TO_TILT_VALUE:
			break;
		case ZCL_CMD_GO_TO_TILT_PERCENTAGE:
			tuyaShutter_moveToTiltPercentageProcess(cmdId, (zcl_goToTiltPer_t *)cmdPayload);
			break;
		default:
			status = ZCL_STA_UNSUP_CLUSTER_COMMAND;
			break;
		}
	}

	return status;
}

#endif /* __PROJECT_TL_ROLLER_SHUTTER__ */
