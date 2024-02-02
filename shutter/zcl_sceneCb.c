/********************************************************************************************************
 * @file    zcl_sceneCb.c
 *
 * @brief   This is the source file for zcl_sceneCb
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


/*********************************************************************
 * @fn      tuyaShutter_sceneRecallReqHandler
 *
 * @brief   Handler for ZCL scene recall command. This function will recall scene.
 *
 * @param	pApsdeInd
 * @param	pScene
 *
 * @return  None
 */
static void tuyaShutter_sceneRecallReqHandler(zclIncomingAddrInfo_t *pAddrInfo, zcl_sceneEntry_t *pScene)
{
	u8 extLen = 0;


}

/*********************************************************************
 * @fn      tuyaShutter_sceneStoreReqHandler
 *
 * @brief   Handler for ZCL scene store command. This function will set scene attribute first.
 *
 * @param   pApsdeInd
 * @param   pScene
 *
 * @return  None
 */
static void tuyaShutter_sceneStoreReqHandler(zcl_sceneEntry_t *pScene)
{
	/* receive Store Scene Request command, get the latest Scene info to save */
	u8 extLen = 0;

	pScene->extFieldLen = extLen;
}

/*********************************************************************
 * @fn      tuyaShutter_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pApsdeInd
 *
 * @return  None
 */
status_t tuyaShutter_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == TUYA_SHUTTER_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_SCENE_STORE_SCENE:
					tuyaShutter_sceneStoreReqHandler((zcl_sceneEntry_t *)cmdPayload);
					break;
				case ZCL_CMD_SCENE_RECALL_SCENE:
					tuyaShutter_sceneRecallReqHandler(pAddrInfo, (zcl_sceneEntry_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}

#endif  /* __PROJECT_TL_ROLLER_SHUTTER__ */
