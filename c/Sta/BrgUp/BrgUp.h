/**
 * @file BrgUp.h
 * @brief 上电启动配置模块。
 * @details 定义上电启动（Bring-Up）的状态枚举。各项目差异化的启动状态在此定义，框架部分（Sta）无需修改。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef BRG_UP_H
#define BRG_UP_H

#include "Typ.h"

/*****************************************************************************
 * 类型定义                                                                  *
 *****************************************************************************/
/* ===== 枚举定义 ===== */
/**
 * @enum enBrgUp
 * @brief 上电启动状态枚举。
 * @details 上电启动为线性流程，各状态依次前进；某一步失败则停留该步，不再向下跳转。
 */
typedef enum
{
    BRG_UP_CHP_INIT,     //!< 片级初始化状态。
    BRG_UP_BRD_INIT,     //!< 板级初始化状态。
    BRG_UP_VLT_CHK,      //!< 电压检查状态。
    BRG_UP_TMP_CHK,      //!< 温度检查状态。
    BRG_UP_PERI_STA_CHK, //!< 外围设备状态检查状态。
    BRG_UP_CAL_LD,       //!< 标定数据加载状态。
    BRG_UP_CTL_STA_CHK,  //!< 被控设备状态检查状态。
    BRG_UP_CTL_STRT,     //!< 启动被控设备状态。
    BRG_UP_CTL_STBL,     //!< 等待被控设备稳定状态。
    BRG_UP_DONE,         //!< 上电启动完成状态。

    BRG_UP_AMT,          //!< 状态数量。
} enBrgUp;

/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化上电启动状态机。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态个数为 0 或初始状态枚举值越界。
 */
err erInitBrgUp(void);

/**
 * @brief 上电启动状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckBrgUp(void);

/**
 * @brief 获取当前上电启动状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurBrgUp(enBrgUp* const kpeCurSta);

#endif // BRG_UP_H
