/********************************************************************************************************
 * @file    zcl_tuyaShutterCb.c
 *
 * @brief   This is the source file for zcl_tuyaShutterCb
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
#include "ota.h"
#include "tuyaShutter.h"
#include "tuyaShutterCtrl.h"
#include "app_ui.h"

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */

/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void tuyaShutter_zclReadCmd(zclReadCmd_t *pReadCmd);
static void tuyaShutter_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void tuyaShutter_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd);
static void tuyaShutter_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd);
#endif
#ifdef ZCL_REPORT
static void tuyaShutter_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd);
static void tuyaShutter_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void tuyaShutter_zclReportCmd(zclReportCmd_t *pReportCmd);
#endif
static void tuyaShutter_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd);

/**********************************************************************
 * GLOBAL VARIABLES
 */

/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      tuyaShutter_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void tuyaShutter_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	//u8 status = ZCL_STA_SUCCESS;

	printf("tuyaShutter_zclProcessIncomingMsg %d\n", pInHdlrMsg->hdr.cmd);

	switch (pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
	case ZCL_CMD_READ:
//		status = zcl_readHandler(pInHdlrMsg);
		tuyaShutter_zclReadCmd(pInHdlrMsg->attrCmd);
		break;
	// case ZCL_CMD_READ_RSP:
	// 	status = zcl_readRspHandler(pCmd);
	// 	break;


	case ZCL_CMD_READ_RSP:
		tuyaShutter_zclReadRspCmd(pInHdlrMsg->attrCmd);
		break;
#endif
#ifdef ZCL_WRITE
	case ZCL_CMD_WRITE:
		tuyaShutter_zclWriteReqCmd(pInHdlrMsg->msg->indInfo.cluster_id, pInHdlrMsg->attrCmd);
		break;
	case ZCL_CMD_WRITE_RSP:
		tuyaShutter_zclWriteRspCmd(pInHdlrMsg->attrCmd);
		break;
#endif
#ifdef ZCL_REPORT
	case ZCL_CMD_CONFIG_REPORT:
		tuyaShutter_zclCfgReportCmd(pInHdlrMsg->attrCmd);
		break;
	case ZCL_CMD_CONFIG_REPORT_RSP:
		tuyaShutter_zclCfgReportRspCmd(pInHdlrMsg->attrCmd);
		break;
	case ZCL_CMD_REPORT:
		tuyaShutter_zclReportCmd(pInHdlrMsg->attrCmd);
		break;
#endif
	case ZCL_CMD_DEFAULT_RSP:
		tuyaShutter_zclDfltRspCmd(pInHdlrMsg->attrCmd);
		break;
	default:
		break;
	}
//	return status;
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      tuyaShutter_zclReadCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclReadCmd(zclReadCmd_t *pReadCmd)
{
	printf("tuyaShutter_zclReadCmd\n");

}

/*********************************************************************
 * @fn      tuyaShutter_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclReadRspCmd(zclReadRspCmd_t *pReadRspCmd)
{
	printf("tuyaShutter_zclReadRspCmd\n");
}
#endif

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      tuyaShutter_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param
 *
 * @return  None
 */
static void tuyaShutter_zclWriteReqCmd(u16 clusterId, zclWriteCmd_t *pWriteReqCmd)
{
	printf("tuyaShutter_zclWriteReqCmd\n");
	u8 numAttr = pWriteReqCmd->numAttr;
	zclWriteRec_t *attr = pWriteReqCmd->attrList;

	if (clusterId == ZCL_CLUSTER_CLOSURES_WINDOW_COVERING)
	{
	// 	for (u8 i = 0; i < numAttr; i++)
	// 	{
	// 		if (attr[i].attrID == ZCL_ATTRID_START_UP_ONOFF)
	// 		{ //???????????
	// 			zcl_WindowCoveringAttr_save();
	// 		}
	// 	}
		for (u8 i = 0; i < numAttr; i++)
		{
			printf("WriteReq: %d %d %d\n", i, attr[i].attrID, *(attr[i].attrData));
			switch (attr[i].attrID)
			{
				case ZCL_ATTRID_MODE:
					if(*(attr[i].attrData) & 0x1)
					{
						//zcl_setAttrVal(TUYA_SHUTTER_ENDPOINT, ZCL_CLUSTER_CLOSURES_WINDOW_COVERING,attr[i].attrID,attr[i].attrData); 
						g_zcl_WindowCoveringAttrs.ConfigStatus |= 0x4;
						g_zcl_nv_WindowCovering.ReverseFlags |= 0x1;
					}
					else
					{
						g_zcl_WindowCoveringAttrs.ConfigStatus &= ~0x4;
						g_zcl_nv_WindowCovering.ReverseFlags &= ~0x1;
					}
					gShutterCtx.shutterAttrsChanged = TRUE;
			 	break;
				case 0x200:
				case 0x201:
				case 0x202:
				case 0x203:
				case 0x204:
					gShutterCtx.shutterAttrsChanged = TRUE;
					break;
			}
			//zcl_setAttrVal(TUYA_SHUTTER_ENDPOINT, ZCL_CLUSTER_CLOSURES_WINDOW_COVERING,attr[i].attrID,attr[i].attrData); 
		}
	}
}

/*********************************************************************
 * @fn      tuyaShutter_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclWriteRspCmd(zclWriteRspCmd_t *pWriteRspCmd)
{

	printf("tuyaShutter_zclWriteRspCmd\n");
}
#endif

/*********************************************************************
 * @fn      tuyaShutter_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclDfltRspCmd(zclDefaultRspCmd_t *pDftRspCmd)
{
	printf("tuyaShutter_zclDfltRspCmd\n");
#ifdef ZCL_OTA
	if ((pDftRspCmd->commandID == ZCL_CMD_OTA_UPGRADE_END_REQ) &&
		(pDftRspCmd->statusCode == ZCL_STA_ABORT))
	{
		if (zcl_attr_imageUpgradeStatus == IMAGE_UPGRADE_STATUS_DOWNLOAD_COMPLETE)
		{
			ota_upgradeAbort();
		}
	}
#endif
}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      tuyaShutter_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclCfgReportCmd(zclCfgReportCmd_t *pCfgReportCmd)
{
	printf("tuyaShutter_zclCfgReportCmd\n");
}

/*********************************************************************
 * @fn      tuyaShutter_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclCfgReportRspCmd(zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
	printf("tuyaShutter_zclCfgReportRspCmd\n");
}

/*********************************************************************
 * @fn      tuyaShutter_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void tuyaShutter_zclReportCmd(zclReportCmd_t *pReportCmd)
{
	printf("tuyaShutter_zclReportCmd\n");
}
#endif

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      tuyaShutter_basicCb
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t tuyaShutter_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if (cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT)
	{
		// Reset all the attributes of all its clusters to factory defaults
		// zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif

#ifdef ZCL_IDENTIFY
s32 tuyaShutter_zclIdentifyTimerCb(void *arg)
{
	if (g_zcl_identifyAttrs.identifyTime <= 0)
	{
		light_blink_stop();

		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void tuyaShutter_zclIdentifyTimerStop(void)
{
	if (identifyTimerEvt)
	{
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      tuyaShutter_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void tuyaShutter_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if (identifyTime == 0)
	{
		tuyaShutter_zclIdentifyTimerStop();
		light_blink_stop();
	}
	else
	{
		if (!identifyTimerEvt)
		{
			light_blink_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(tuyaShutter_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      tuyaShutter_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void tuyaShutter_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	u8 effectId = pTriggerEffect->effectId;
	//	u8 effectVariant = pTriggerEffect->effectVariant;

	switch (effectId)
	{
	case IDENTIFY_EFFECT_BLINK:
		light_blink_start(1, 500, 500);
		break;
	case IDENTIFY_EFFECT_BREATHE:
		light_blink_start(15, 300, 700);
		break;
	case IDENTIFY_EFFECT_OKAY:
		light_blink_start(2, 250, 250);
		break;
	case IDENTIFY_EFFECT_CHANNEL_CHANGE:
		light_blink_start(1, 500, 7500);
		break;
	case IDENTIFY_EFFECT_FINISH_EFFECT:
		light_blink_start(1, 300, 700);
		break;
	case IDENTIFY_EFFECT_STOP_EFFECT:
		light_blink_stop();
		break;
	default:
		break;
	}
}

/*********************************************************************
 * @fn      tuyaShutter_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t tuyaShutter_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	if (pAddrInfo->dstEp == TUYA_SHUTTER_ENDPOINT)
	{
		if (pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR)
		{
			switch (cmdId)
			{
			case ZCL_CMD_IDENTIFY:
				tuyaShutter_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
				break;
			case ZCL_CMD_TRIGGER_EFFECT:
				tuyaShutter_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
				break;
			default:
				break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif

#endif /* __PROJECT_TL_ROLLER_SHUTTER__ */
