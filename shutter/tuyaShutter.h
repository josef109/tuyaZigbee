/********************************************************************************************************
 * @file    tuyaShutter.h
 *
 * @brief   This is the header file for tuyaShutter
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

#ifndef _TUYA_LIGHT_H_
#define _TUYA_LIGHT_H_



/**********************************************************************
 * CONSTANT
 */
#define TUYA_SHUTTER_ENDPOINT   0x01
#define SAMPLE_TEST_ENDPOINT	0x02

enum zb_zcl_window_covering_window_covering_type_e
{
  /** Rollershade value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE                       = 0x00,
  /** Rollershade - 2 Motor value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_2_MOTOR               = 0x01,
  /** Rollershade - Exterior value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_EXTERIOR              = 0x02,
  /** Rollershade - Exterior - 2 Motor value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_ROLLERSHADE_EXTERIOR_2_MOTOR      = 0x03,
  /** Drapery value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_DRAPERY                           = 0x04,
  /** Awning value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_AWNING                            = 0x05,
  /** Shutter value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_SHUTTER                           = 0x06,
  /** Tilt Blind - Tilt Only value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_TILT_BLIND_TILT_ONLY              = 0x07,
  /** Tilt Blind - Lift and Tilt value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_TILT_BLIND_LIFT_AND_TILT          = 0x08,
  /** Projector screen value */
  ZB_ZCL_ATTR_WINDOW_COVERING_TYPE_PROJECTOR_SCREEN                  = 0x09
};


/**********************************************************************
 * TYPEDEFS
 */
typedef struct{
	u8 keyType; /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
	 	 	 	 SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
	u8 key[16];	/* the key used */
}app_linkKey_info_t;

typedef struct{
	ev_timer_event_t *timerLedEvt;
	u32 keyPressedTime;
	u32 keyPressedTimeUpDown;

	u16 ledOnTime;
	u16 ledOffTime;
	u8 	oriSta;		//original state before blink
	u8 	sta;		//current state in blink
	u8 	times;		//blink times
	u8  state;

	bool bdbFindBindFlg;
	bool shutterAttrsChanged;

	app_linkKey_info_t tcLinkKey;
}app_ctx_t;

/**
 *  @brief Defined for basic cluster attributes
 */
typedef struct{
	u8 	zclVersion;                         //attr 0
	u8	appVersion;                         //attr 1
	u8	stackVersion;                       //attr 2
	u8	hwVersion;                          //attr 3
	u8	manuName[ZCL_BASIC_MAX_LENGTH];     //attr 4
	u8	modelId[ZCL_BASIC_MAX_LENGTH];      //attr 5
	u8  dateCode[ZCL_BASIC_MAX_LENGTH];     //attr 6
	u8	powerSource;                        //attr 7
	u8	deviceEnable;                       //attr 12
	u8  swBuildId[ZCL_BASIC_MAX_LENGTH];    //attr 4000
}zcl_basicAttr_t;

/**
 *  @brief Defined for identify cluster attributes
 */
typedef struct{
	u16	identifyTime;
}zcl_identifyAttr_t;

/**
 *  @brief Defined for group cluster attributes
 */
typedef struct{
	u8	nameSupport;
}zcl_groupAttr_t;

/**
 *  @brief Defined for scene cluster attributes
 */
typedef struct{
	u8	 sceneCount;
	u8	 currentScene;
	u8	 nameSupport;
	bool sceneValid;
	u16	 currentGroup;
}zcl_sceneAttr_t;


#define NV_ITEM_ZCL_WINDOWCOVERING	(NV_ITEM_APP_GP_TRANS_TABLE + 1)

/**
 *  @brief Defined for saving level attributes
 */
typedef struct {
    u16 LiftTimeUp;
	u16 LiftTimeDown;
	u8 ReverseWaitTime;
	u8 TiltMoveTime;
	u8 ReverseFlags;
}zcl_nv_WindowCovering_t;

typedef struct
{
	u8 WindowCoveringType;
	u16 PhysicalClosedLimitLift;
	u16 PhysicalClosedLimitTilt;
	u16 CurrentPositionLift;
	u16 CurrentPositionTilt;
	u16 NumOfActuationsLift;
	u16 NumOfActuationsTilt;
	u8 ConfigStatus;
	u8 CurrentPositionLiftPercentage;
	u8 CurrentPositionTiltPercentage;
	u16 InstalledOpenLimitLift;
	u16 InstalledClosedLimitLift;
	u16 InstalledOpenLimitTilt;
	u16 InstalledClosedLimitTilt;
	u16 VelocityLift;
	u16 AccelerationTimeLift;
	u16 DecelerationTimeLift;
	u8 Mode;
	u8 IntermediateSetpointsLift[3];  // “1,0x0000”
	u8 IntermediateSetpointsTilt[3];  // “1,0x0000”
    // u16 LiftTimeUp;
	// u16 LiftTimeDown;
	// u8 ReverseWaitTime;
	// u8 TiltMoveTime;
	// u8 ReverseFlags;
} zcl_WindowCoveringAttr_t;

/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t gShutterCtx;
extern bdb_commissionSetting_t g_bdbCommissionSetting;
extern bdb_appCb_t g_zbShutterBdbCb;


extern u8 TUYASHUTTER_CB_CLUSTER_NUM;
extern const zcl_specClusterInfo_t g_tuyaShutterClusterList[];
extern const af_simple_descriptor_t tuyaShutter_simpleDesc;
#if AF_TEST_ENABLE
extern const af_simple_descriptor_t sampleTestDesc;
#endif



/* Attributes */
extern zcl_basicAttr_t g_zcl_basicAttrs;
extern zcl_identifyAttr_t g_zcl_identifyAttrs;
extern zcl_groupAttr_t g_zcl_groupAttrs;
extern zcl_sceneAttr_t g_zcl_sceneAttrs;
extern zcl_WindowCoveringAttr_t g_zcl_WindowCoveringAttrs;
extern zcl_nv_WindowCovering_t g_zcl_nv_WindowCovering;

#define zcl_sceneAttrGet()		&g_zcl_sceneAttrs
#define zcl_WindowCoveringGet()		&g_zcl_WindowCoveringAttrs


/**********************************************************************
 * FUNCTIONS
 */
void tuyaShutter_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg);

status_t tuyaShutter_basicCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaShutter_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaShutter_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t tuyaShutter_windowCoveringCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);


void tuyaShutter_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void tuyaShutter_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);
void tuyaShutter_otaProcessMsgHandler(u8 evt, u8 status);
bool tuyaShutter_nwkUpdateIndicateHandler(nwkCmd_nwkUpdate_t *pNwkUpdate);

void tuyaShutter_onoff(u8 cmd);

void zcl_tuyaShutterAttrsInit(void);
//nv_sts_t zcl_PositionTiltPercentageAttr_save(void);
nv_sts_t zcl_WindowCoveringAttr_save(void);

void tuyaShutter_coverInit(void);
void tuyaShutter_openProcess(void);
void tuyaShutter_closeProcess(void);
void tuyaShutter_stopProcess(void);
bool moveFlag(void);
nv_sts_t zcl_WindowCoveringAttr_restore(void);

#if AF_TEST_ENABLE
void afTest_rx_handler(void *arg);
void afTest_dataSendConfirm(void *arg);
#endif

#endif /* _TUYA_LIGHT_H_ */
