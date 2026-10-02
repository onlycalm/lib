/**
 * @file BrgDn.h
 * @brief 下电启动配置模块。
 * @details 定义下电启动（Bring-Down）的状态枚举。各项目差异化的关断状态在此定义，框架部分（Sta）无需修改。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef BRG_DN_H
#define BRG_DN_H

#include "Typ.h"

/*****************************************************************************
 * 类型定义                                                                  *
 *****************************************************************************/
/* ===== 枚举定义 ===== */
/**
 * @enum enBrgDn
 * @brief 下电启动状态枚举。
 * @details 下电启动为线性流程，各状态依次前进；某一步失败则停留该步，不再向下跳转。
 */
typedef enum
{
    BRG_DN_CTL_STP,      //!< 停止被控设备状态。
    BRG_DN_CTL_STP_WAIT, //!< 等待被控设备完全停止状态。
    BRG_DN_PERI_STP,     //!< 停止操作外围设备状态。
    BRG_DN_STG_WAIT,     //!< 等待存储完成状态。
    BRG_DN_DONE,         //!< 关断完成状态。

    BRG_DN_AMT,          //!< 状态数量。
} enBrgDn;

/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化下电启动状态机。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态个数为 0 或初始状态枚举值越界。
 */
err erInitBrgDn(void);

/**
 * @brief 下电启动状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckBrgDn(void);

/**
 * @brief 获取当前下电启动状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurBrgDn(enBrgDn* const kpeCurSta);

#endif // BRG_DN_H
