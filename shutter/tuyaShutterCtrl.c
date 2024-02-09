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






#endif	/* __PROJECT_TL_ROLLER_SHUTTER__ */
