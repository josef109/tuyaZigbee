/********************************************************************************************************
 * @file    tuyaShutterEpCfg.c
 *
 * @brief   This is the source file for tuyaShutterEpCfg
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

/**********************************************************************
 * LOCAL CONSTANTS
 */

/**********************************************************************
 * TYPEDEFS
 */

/**********************************************************************
 * GLOBAL VARIABLES
 */
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 tuyaShutter_inClusterList[] =
	{
		ZCL_CLUSTER_GEN_BASIC,
		ZCL_CLUSTER_GEN_IDENTIFY,
#ifdef ZCL_GROUP
		ZCL_CLUSTER_GEN_GROUPS,
#endif
#ifdef ZCL_SCENE
		ZCL_CLUSTER_GEN_SCENES,
#endif
		ZCL_CLUSTER_CLOSURES_WINDOW_COVERING,
#ifdef ZCL_ZLL_COMMISSIONING
		ZCL_CLUSTER_TOUCHLINK_COMMISSIONING,
#endif
#ifdef ZCL_WWAH
		ZCL_CLUSTER_WWAH,
#endif
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 tuyaShutter_outClusterList[] =
	{
#ifdef ZCL_OTA
		ZCL_CLUSTER_OTA,
#endif
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define TUYASHUTTER_IN_CLUSTER_NUM (sizeof(tuyaShutter_inClusterList) / sizeof(tuyaShutter_inClusterList[0]))
#define TUYASHUTTER_OUT_CLUSTER_NUM (sizeof(tuyaShutter_outClusterList) / sizeof(tuyaShutter_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t tuyaShutter_simpleDesc =
	{
		HA_PROFILE_ID, /* Application profile identifier */
		HA_DEV_WINDOW_COVERING_DEVICE,
		TUYA_SHUTTER_ENDPOINT,			   /* Endpoint */
		1,								   /* Application device version */
		0,								   /* Reserved */
		TUYASHUTTER_IN_CLUSTER_NUM,		   /* Application input cluster count */
		TUYASHUTTER_OUT_CLUSTER_NUM,	   /* Application output cluster count */
		(u16 *)tuyaShutter_inClusterList,  /* Application input cluster list */
		(u16 *)tuyaShutter_outClusterList, /* Application output cluster list */
};

#if AF_TEST_ENABLE
/**
 *  @brief Definition for Incoming cluster / Sever Cluster
 */
const u16 sampleTest_inClusterList[] =
	{
		ZCL_CLUSTER_TELINK_SDK_TEST_REQ,
		ZCL_CLUSTER_TELINK_SDK_TEST_RSP,
		ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_REQ,
		ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_RSP,
};

/**
 *  @brief Definition for Outgoing cluster / Client Cluster
 */
const u16 sampleTest_outClusterList[] =
	{
		ZCL_CLUSTER_TELINK_SDK_TEST_REQ,
		ZCL_CLUSTER_TELINK_SDK_TEST_RSP,
		ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_REQ,
		ZCL_CLUSTER_TELINK_SDK_TEST_CLEAR_RSP,
};

/**
 *  @brief Definition for Server cluster number and Client cluster number
 */
#define SAMPLE_TEST_IN_CLUSTER_NUM (sizeof(sampleTest_inClusterList) / sizeof(sampleTest_inClusterList[0]))
#define SAMPLE_TEST_OUT_CLUSTER_NUM (sizeof(sampleTest_outClusterList) / sizeof(sampleTest_outClusterList[0]))

/**
 *  @brief Definition for simple description for HA profile
 */
const af_simple_descriptor_t sampleTestDesc =
	{
		HA_PROFILE_ID,					  /* Application profile identifier */
		HA_DEV_WINDOW_COVERING_DEVICE,	  /* Application device identifier */
		SAMPLE_TEST_ENDPOINT,			  /* Endpoint */
		0,								  /* Application device version */
		0,								  /* Reserved */
		SAMPLE_TEST_IN_CLUSTER_NUM,		  /* Application input cluster count */
		SAMPLE_TEST_OUT_CLUSTER_NUM,	  /* Application output cluster count */
		(u16 *)sampleTest_inClusterList,  /* Application input cluster list */
		(u16 *)sampleTest_outClusterList, /* Application output cluster list */
};
#endif /* AF_TEST_ENABLE */

/* Basic */
zcl_basicAttr_t g_zcl_basicAttrs =
	{
		.zclVersion = 0x03, // ZCL v7
		.appVersion = (APP_RELEASE | APP_BUILD),
		.stackVersion = (STACK_RELEASE | STACK_BUILD),
		.hwVersion = HARDWARE_REV,
		.manuName = ZCL_BASIC_MFG_NAME,
		.modelId = ZCL_BASIC_MODEL_ID,
		.dateCode = ZCL_BASIC_DATE_CODE,
		.powerSource = POWER_SOURCE_DC,
		.swBuildId = ZCL_BASIC_SW_BUILD_ID,
		.deviceEnable = TRUE,
};

const zclAttrInfo_t basic_attrTbl[] =
	{
		{ZCL_ATTRID_BASIC_ZCL_VER, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.zclVersion},
		{ZCL_ATTRID_BASIC_APP_VER, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.appVersion},
		{ZCL_ATTRID_BASIC_STACK_VER, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.stackVersion},
		{ZCL_ATTRID_BASIC_HW_VER, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.hwVersion},
		{ZCL_ATTRID_BASIC_MFR_NAME, ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ, (u8 *)g_zcl_basicAttrs.manuName},
		{ZCL_ATTRID_BASIC_MODEL_ID, ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ, (u8 *)g_zcl_basicAttrs.modelId},
		{ZCL_ATTRID_BASIC_POWER_SOURCE, ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.powerSource},
		{ZCL_ATTRID_BASIC_DEV_ENABLED, ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&g_zcl_basicAttrs.deviceEnable},
		{ZCL_ATTRID_BASIC_SW_BUILD_ID, ZCL_DATA_TYPE_CHAR_STR, ACCESS_CONTROL_READ, (u8 *)&g_zcl_basicAttrs.swBuildId},

		{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&zcl_attr_global_clusterRevision},
};

#define ZCL_BASIC_ATTR_NUM sizeof(basic_attrTbl) / sizeof(zclAttrInfo_t)

/* Identify */
zcl_identifyAttr_t g_zcl_identifyAttrs =
	{
		.identifyTime = 0x0000,
};

const zclAttrInfo_t identify_attrTbl[] =
	{
		{ZCL_ATTRID_IDENTIFY_TIME, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&g_zcl_identifyAttrs.identifyTime},

		{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&zcl_attr_global_clusterRevision},
};

#define ZCL_IDENTIFY_ATTR_NUM sizeof(identify_attrTbl) / sizeof(zclAttrInfo_t)

#ifdef ZCL_GROUP
/* Group */
zcl_groupAttr_t g_zcl_groupAttrs =
	{
		.nameSupport = 0,
};

const zclAttrInfo_t group_attrTbl[] =
	{
		{ZCL_ATTRID_GROUP_NAME_SUPPORT, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_groupAttrs.nameSupport},

		{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&zcl_attr_global_clusterRevision},
};

#define ZCL_GROUP_ATTR_NUM sizeof(group_attrTbl) / sizeof(zclAttrInfo_t)
#endif

#ifdef ZCL_SCENE
/* Scene */
zcl_sceneAttr_t g_zcl_sceneAttrs =
	{
		.sceneCount = 0,
		.currentScene = 0,
		.currentGroup = 0x0000,
		.sceneValid = FALSE,
		.nameSupport = 0,
};

const zclAttrInfo_t scene_attrTbl[] =
	{
		{ZCL_ATTRID_SCENE_SCENE_COUNT, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_sceneAttrs.sceneCount},
		{ZCL_ATTRID_SCENE_CURRENT_SCENE, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_sceneAttrs.currentScene},
		{ZCL_ATTRID_SCENE_CURRENT_GROUP, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&g_zcl_sceneAttrs.currentGroup},
		{ZCL_ATTRID_SCENE_SCENE_VALID, ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ, (u8 *)&g_zcl_sceneAttrs.sceneValid},
		{ZCL_ATTRID_SCENE_NAME_SUPPORT, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_sceneAttrs.nameSupport},

		{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&zcl_attr_global_clusterRevision},
};

#define ZCL_SCENE_ATTR_NUM sizeof(scene_attrTbl) / sizeof(zclAttrInfo_t)
#endif

zcl_WindowCoveringAttr_t g_zcl_WindowCoveringAttrs =
	{
		.WindowCoveringType = ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_TILT_BLIND_LIFT_AND_TILT,
		.PhysicalClosedLimitLift = 0,
		.PhysicalClosedLimitTilt = 0,
		.CurrentPositionLift = 0,
		.CurrentPositionTilt = 0,
		.NumOfActuationsLift = 0,
		.NumOfActuationsTilt = 0,
		.ConfigStatus = 3,
		.CurrentPositionLiftPercentage = 0,
		.CurrentPositionTiltPercentage = 0,
		.InstalledOpenLimitLift = 0,
		.InstalledClosedLimitLift = 0xffff,
		.InstalledOpenLimitTilt = 0,
		.InstalledClosedLimitTilt = 0xffff,
		.VelocityLift = 0,
		.AccelerationTimeLift = 0,
		.DecelerationTimeLift = 0,
		.Mode = 0x14,
		.IntermediateSetpointsLift = {1, 0, 0}, // “1,0x0000”
		.IntermediateSetpointsTilt = {1, 0, 0}, // “1,0x0000”
		.LiftTimeUp = 570,
		.LiftTimeDown = 570,
		.ReverseWaitTime = 15,
		.TiltMoveTime = 16,
		.ReverseFlags = 0,
};

// /**********************************************************************
//  * LOCAL VARIABLES
//  */
// #ifdef ZCL_WINDOW_COVERING

// /* Attribute default */
// u8 zcl_attr_WindowCoveringType = 0;
// u8 zcl_attr_ConfigStatus = 0x03;
// u8 zcl_attr_Mode = 0x04;


// /* Attribute record list */
// const zclAttrInfo_t windowCovering_attrTbl[] =
// {
//     { ZCL_ATTRID_WINDOW_COVERING_TYPE,	   	ZCL_DATA_TYPE_ENUM8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_WindowCoveringType},
//     { ZCL_ATTRID_CONFIG_STATUS,	   			ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_ConfigStatus},
// 	{ ZCL_ATTRID_MODE,	   					ZCL_DATA_TYPE_BITMAP8,     	ACCESS_CONTROL_READ| ACCESS_CONTROL_WRITE,  (u8*)&zcl_attr_Mode},
// 	{ ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, 	ZCL_DATA_TYPE_UINT16,  		ACCESS_CONTROL_READ,  						(u8*)&zcl_attr_global_clusterRevision},
// };

// const u8 zcl_windowCovering_attrNum = ( sizeof(windowCovering_attrTbl) / sizeof(zclAttrInfo_t) );

// #endif    /* ZCL_IAS_ZONE */
// #endif
static zcl_nv_WindowCovering_t zcl_nv_WindowCovering;


const zclAttrInfo_t window_covering_attrTbl[] =
	{
		{ZCL_ATTRID_WINDOW_COVERING_TYPE, ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.WindowCoveringType},
		{ZCL_ATTRID_CONFIG_STATUS, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.ConfigStatus},
		{ZCL_ATTRID_CURRENT_POSITION_LIFT_PERCENTAGE, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8 *)&g_zcl_WindowCoveringAttrs.CurrentPositionLiftPercentage},
		{ZCL_ATTRID_CURRENT_POSITION_TILT_PERCENTAGE, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, (u8 *)&g_zcl_WindowCoveringAttrs.CurrentPositionTiltPercentage},
		{ZCL_ATTRID_INSTALLED_OPEN_LIMIT_LIFT, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.InstalledOpenLimitLift},
		{ZCL_ATTRID_INSTALLED_CLOSED_LIMIT_LIFT, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.InstalledClosedLimitLift},
		{ZCL_ATTRID_INSTALLED_OPEN_LIMIT_TILT, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.InstalledOpenLimitTilt},
		{ZCL_ATTRID_INSTALLED_CLOSED_LIMIT_TILT, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&g_zcl_WindowCoveringAttrs.InstalledClosedLimitTilt},
		{ZCL_ATTRID_MODE, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&g_zcl_WindowCoveringAttrs.Mode},

		// {0x20, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.LiftTimeUp},
		// {0x21, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.LiftTimeDown},
		// {0x22, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.ReverseWaitTime},
		// {0x23, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.TiltMoveTime},
		// {0x24, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.ReverseFlags},

		{ZCL_ATTRID_GLOBAL_CLUSTER_REVISION, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ, (u8 *)&zcl_attr_global_clusterRevision},

};

#define ZCL_WINDOW_COVERING_ATTR_NUM sizeof(window_covering_attrTbl) / sizeof(zclAttrInfo_t)

_CODE_ZCL_ static status_t zcl_custom_clientCmdHandler(zclIncoming_t *pInMsg)
{
		u8 status = ZCL_STA_SUCCESS;
		return status;
}

_CODE_ZCL_ static status_t zcl_custom_cmdHandler(zclIncoming_t *pInMsg)
{
	if(pInMsg->hdr.frmCtrl.bf.dir == ZCL_FRAME_CLIENT_SERVER_DIR){
		return zcl_custom_clientCmdHandler(pInMsg);
	}else{
		return ZCL_STA_UNSUP_CLUSTER_COMMAND;
	}
}

_CODE_ZCL_ status_t zcl_custom_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
	return zcl_registerCluster(endpoint, ZCL_CLUSTER_TELINK_SDK_TEST, manuCode, attrNum, attrTbl, zcl_custom_cmdHandler, cb);
}

status_t tuyaShutter_customCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
	u8 status = ZCL_STA_FAILURE;

	return status;
}


const zclAttrInfo_t custom_attrTbl[] =
	{
		{0, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.LiftTimeUp},
		{1, ZCL_DATA_TYPE_UINT16, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.LiftTimeDown},
		{2, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.ReverseWaitTime},
		{3, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.TiltMoveTime},
		{4, ZCL_DATA_TYPE_UINT8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, (u8 *)&zcl_nv_WindowCovering.ReverseFlags},
};

#define ZCL_CUSTOM_ATTR_NUM sizeof(custom_attrTbl) / sizeof(zclAttrInfo_t)


/**
 *  @brief Definition for simple light ZCL specific cluster
 */
const zcl_specClusterInfo_t g_tuyaShutterClusterList[] =
	{
		{ZCL_CLUSTER_GEN_BASIC, MANUFACTURER_CODE_NONE, ZCL_BASIC_ATTR_NUM, basic_attrTbl, zcl_basic_register, tuyaShutter_basicCb},
		{ZCL_CLUSTER_GEN_IDENTIFY, MANUFACTURER_CODE_NONE, ZCL_IDENTIFY_ATTR_NUM, identify_attrTbl, zcl_identify_register, tuyaShutter_identifyCb},
#ifdef ZCL_GROUP
		{ZCL_CLUSTER_GEN_GROUPS, MANUFACTURER_CODE_NONE, ZCL_GROUP_ATTR_NUM, group_attrTbl, zcl_group_register, NULL},
#endif
#ifdef ZCL_SCENE
		{ZCL_CLUSTER_GEN_SCENES, MANUFACTURER_CODE_NONE, ZCL_SCENE_ATTR_NUM, scene_attrTbl, zcl_scene_register, tuyaShutter_sceneCb},
#endif
		{ZCL_CLUSTER_CLOSURES_WINDOW_COVERING, MANUFACTURER_CODE_NONE, ZCL_WINDOW_COVERING_ATTR_NUM, window_covering_attrTbl, zcl_windowCovering_register, tuyaShutter_windowCoveringCb},
//		{0xeeee, 1, ZCL_CUSTOM_ATTR_NUM, custom_attrTbl, zcl_custom_register, tuyaShutter_customCb},

};

u8 TUYASHUTTER_CB_CLUSTER_NUM = (sizeof(g_tuyaShutterClusterList) / sizeof(g_tuyaShutterClusterList[0]));


/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      zcl_WindowCoveringAttr_save
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_WindowCoveringAttr_save(void)
{
	nv_sts_t st = NV_SUCC;

#if NV_ENABLE
	zcl_nv_WindowCovering_t zcl_nv_WindowCovering;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WINDOWCOVERING, sizeof(zcl_nv_WindowCovering_t), (u8 *)&zcl_nv_WindowCovering);

	if ((st == NV_ITEM_NOT_FOUND) ||
		((st == NV_SUCC) && ((zcl_nv_WindowCovering.LiftTimeDown != g_zcl_WindowCoveringAttrs.LiftTimeDown) ||
							 (zcl_nv_WindowCovering.LiftTimeUp != g_zcl_WindowCoveringAttrs.LiftTimeUp) ||
							 (zcl_nv_WindowCovering.ReverseWaitTime != g_zcl_WindowCoveringAttrs.ReverseWaitTime) ||
							 (zcl_nv_WindowCovering.TiltMoveTime != g_zcl_WindowCoveringAttrs.TiltMoveTime) ||
							 (zcl_nv_WindowCovering.ReverseFlags != g_zcl_WindowCoveringAttrs.ReverseFlags))))
	{
		zcl_nv_WindowCovering.LiftTimeDown = g_zcl_WindowCoveringAttrs.LiftTimeDown;
		zcl_nv_WindowCovering.LiftTimeUp = g_zcl_WindowCoveringAttrs.LiftTimeUp;
		zcl_nv_WindowCovering.ReverseWaitTime = g_zcl_WindowCoveringAttrs.ReverseWaitTime;
		zcl_nv_WindowCovering.TiltMoveTime = g_zcl_WindowCoveringAttrs.TiltMoveTime;
		zcl_nv_WindowCovering.ReverseFlags = g_zcl_WindowCoveringAttrs.ReverseFlags;

		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WINDOWCOVERING, sizeof(zcl_nv_WindowCovering_t), (u8 *)&zcl_nv_WindowCovering);
	}

#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_PositionLiftPercentageAttr_restore
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
nv_sts_t zcl_WindowCoveringAttr_restore(void)
{
	nv_sts_t st = NV_SUCC;

#if NV_ENABLE
	zcl_nv_WindowCovering_t zcl_nv_WindowCovering;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WINDOWCOVERING, sizeof(zcl_nv_WindowCovering_t), (u8 *)&zcl_nv_WindowCovering);

	if (st == NV_SUCC)
	{
		g_zcl_WindowCoveringAttrs.LiftTimeDown = zcl_nv_WindowCovering.LiftTimeDown;
		g_zcl_WindowCoveringAttrs.LiftTimeUp = zcl_nv_WindowCovering.LiftTimeUp;
		g_zcl_WindowCoveringAttrs.ReverseWaitTime = zcl_nv_WindowCovering.ReverseWaitTime;
		g_zcl_WindowCoveringAttrs.TiltMoveTime = zcl_nv_WindowCovering.TiltMoveTime;
		g_zcl_WindowCoveringAttrs.ReverseFlags = zcl_nv_WindowCovering.ReverseFlags;

		if(zcl_nv_WindowCovering.ReverseFlags & 1){
			g_zcl_WindowCoveringAttrs.Mode |= 0x1;
			g_zcl_WindowCoveringAttrs.ConfigStatus |= 0x4;
		}
		else
		{
			g_zcl_WindowCoveringAttrs.Mode &= ~0x1;
			g_zcl_WindowCoveringAttrs.ConfigStatus &= ~0x4;
		}
	}
#else
	st = NV_ENABLE_PROTECT_ERROR;
#endif

	return st;
}

/*********************************************************************
 * @fn      zcl_tuyaShutterAttrsInit
 *
 * @brief
 *
 * @param   None
 *
 * @return
 */
void zcl_tuyaShutterAttrsInit(void)
{
	zcl_WindowCoveringAttr_restore();
	
}

#endif /* __PROJECT_TL_ROLLER_SHUTTER__ */
