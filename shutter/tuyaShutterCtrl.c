/********************************************************************************************************
 * @file    tuyaShutterCtrl.c
 *
 * @brief   This is the source file for tuyaShutterCtrl
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
#include "zcl_include.h"
#include "tuyaShutter.h"
#include "tuyaShutterCtrl.h"


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
 * FUNCTIONS
 */
extern void tuyaShutter_updateOnOff(void);
extern void tuyaShutter_updateLevel(void);
extern void tuyaShutter_updateColor(void);

extern void tuyaShutter_onOffInit(void);
extern void tuyaShutter_levelInit(void);
extern void tuyaShutter_colorInit(void);






/*********************************************************************
 * @fn      light_fresh
 *
 * @brief
 *
 * @param   None
 *
 * @return  None
 */
void light_fresh(void)
{


	tuyaShutter_updateLevel();

	gShutterCtx.lightAttrsChanged = TRUE;
}

/*********************************************************************
 * @fn      light_applyUpdate
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate(u8 *curLevel, u16 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u8 minLevel, u8 maxLevel, bool wrap)
{
	if((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)){
		*curLevel256 = (wrap) ? ((u16)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u16)maxLevel * 256) - 256)
							  : ((u16)maxLevel * 256);
	}else if((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)){
		*curLevel256 = (wrap) ? ((u16)maxLevel * 256 - ((u16)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
							  : ((u16)minLevel * 256);
	}else{
		*curLevel256 += *stepLevel256;
	}

	if(*stepLevel256 > 0){
		*curLevel = (*curLevel256 + 127) / 256;
	}else{
		*curLevel = *curLevel256 / 256;
	}

	if(*remainingTime == 0){
		*curLevel256 = ((u16)*curLevel) * 256;
		*stepLevel256 = 0;
	}else if(*remainingTime != 0xFFFF){
		*remainingTime = *remainingTime -1;
	}

	light_fresh();
}

/*********************************************************************
 * @fn      light_applyUpdate_16
 *
 * @brief
 *
 * @param
 *
 * @return  None
 */
void light_applyUpdate_16(u16 *curLevel, u32 *curLevel256, s32 *stepLevel256, u16 *remainingTime, u16 minLevel, u16 maxLevel, bool wrap)
{
	if((*stepLevel256 > 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) > maxLevel)){
		*curLevel256 = (wrap) ? ((u32)minLevel * 256 + ((*curLevel256 + *stepLevel256) - (u32)maxLevel * 256) - 256)
							  : ((u32)maxLevel * 256);
	}else if((*stepLevel256 < 0) && ((((s32)*curLevel256 + *stepLevel256) / 256) < minLevel)){
		*curLevel256 = (wrap) ? ((u32)maxLevel * 256 - ((u32)minLevel * 256 - ((s32)*curLevel256 + *stepLevel256)) + 256)
							  : ((u32)minLevel * 256);
	}else{
		*curLevel256 += *stepLevel256;
	}

	if(*stepLevel256 > 0){
		*curLevel = (*curLevel256 + 127) / 256;
	}else{
		*curLevel = *curLevel256 / 256;
	}

	if(*remainingTime == 0){
		*curLevel256 = ((u32)*curLevel) * 256;
		*stepLevel256 = 0;
	}else if(*remainingTime != 0xFFFF){
		*remainingTime = *remainingTime -1;
	}

	light_fresh();
}



#endif	/* __PROJECT_TL_ROLLER_SHUTTER__ */
