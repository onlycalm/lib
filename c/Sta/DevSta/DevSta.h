/**
 * @file StaImpl.h
 * @brief 状态机配置模块。
 * @details 定义本项目的状态枚举。各项目的差异化状态在此定义，框架部分无需修改。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef DEV_STA_H
#define DEV_STA_H

#include "Typ.h"

/*****************************************************************************
 * 类型定义                                                                  *
 *****************************************************************************/
/* ===== 枚举定义 ===== */
/**
 * @enum enDevSta
 * @brief 设备状态枚举。
 */
typedef enum
{
    DEV_STA_INIT,    //!< 初始化状态。
    DEV_STA_STBY,    //!< 待机状态。
    DEV_STA_NML_RDY, //!< 常态准备状态。
    DEV_STA_NML,     //!< 常状态。
    DEV_STA_FLT,     //!< 故障状态。
    DEV_STA_PRE_RST, //!< 预复位状态。
    DEV_STA_PRE_SLP, //!< 预睡眠状态。
    DEV_STA_RST,     //!< 复位状态。
    DEV_STA_SLP,     //!< 睡眠状态。

    DEV_STA_AMT,     //!< 状态数量。
} enDevSta;

/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化设备状态机。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态个数为 0 或初始状态枚举值越界。
 */
err erInitDevSta(void);

/**
 * @brief 设备状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckDevSta(void);

/**
 * @brief 获取当前设备状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurDevSta(enDevSta* const kpeCurSta);

#endif // DEV_STA_H
