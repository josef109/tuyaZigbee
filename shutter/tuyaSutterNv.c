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


// void copy_nv(void)
// {
// 	g_zcl_WindowCoveringAttrs.LiftTimeDown = g_zcl_nv_WindowCovering.LiftTimeDown;
// 	g_zcl_WindowCoveringAttrs.LiftTimeUp = g_zcl_nv_WindowCovering.LiftTimeUp;
// 	g_zcl_WindowCoveringAttrs.ReverseWaitTime = g_zcl_nv_WindowCovering.ReverseWaitTime;
// 	g_zcl_WindowCoveringAttrs.TiltMoveTime = g_zcl_nv_WindowCovering.TiltMoveTime;
// 	g_zcl_WindowCoveringAttrs.ReverseFlags = g_zcl_nv_WindowCovering.ReverseFlags;

// }

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
		((st == NV_SUCC) && ((zcl_nv_WindowCovering.LiftTimeDown != g_zcl_nv_WindowCovering.LiftTimeDown) ||
							 (zcl_nv_WindowCovering.LiftTimeUp != g_zcl_nv_WindowCovering.LiftTimeUp) ||
							 (zcl_nv_WindowCovering.ReverseWaitTime != g_zcl_nv_WindowCovering.ReverseWaitTime) ||
							 (zcl_nv_WindowCovering.TiltMoveTime != g_zcl_nv_WindowCovering.TiltMoveTime) ||
							 (zcl_nv_WindowCovering.ReverseFlags != g_zcl_nv_WindowCovering.ReverseFlags))))
	{
		st = nv_flashWriteNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WINDOWCOVERING, sizeof(zcl_nv_WindowCovering_t), (u8 *)&g_zcl_nv_WindowCovering);
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
//	zcl_nv_WindowCovering_t g_zcl_nv_WindowCovering;

	st = nv_flashReadNew(1, NV_MODULE_ZCL, NV_ITEM_ZCL_WINDOWCOVERING, sizeof(zcl_nv_WindowCovering_t), (u8 *)&g_zcl_nv_WindowCovering);

	if (st == NV_SUCC)
	{
		if(g_zcl_nv_WindowCovering.ReverseFlags & 1){
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

#endif