/**
 * @file DevMon.h
 * @brief 模拟量监控配置模块。
 * @details 定义本项目的监控通道与规则枚举。各项目差异化的通道与规则在此定义，
 *          框架部分（Mon）无需修改。
 * @author Calm
 * @data 2026-10-05
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef DEV_MON_H
#define DEV_MON_H

#include "Typ.h"

/*****************************************************************************
 * 类型定义                                                                  *
 *****************************************************************************/
/* ===== 枚举定义 ===== */
/**
 * @enum enDevMonCh
 * @brief 监控通道枚举。
 * @details 通道号即采集数组下标。
 */
typedef enum
{
    DEV_MON_CH_BUS_VOLT, //!< 母线电压通道。
    DEV_MON_CH_BUS_CUR,  //!< 母线电流通道。
    DEV_MON_CH_TEMP,     //!< 温度通道。

    DEV_MON_CH_AMT,      //!< 通道数量。
} enDevMonCh;

/**
 * @enum enDevMonRule
 * @brief 监控规则枚举。
 * @details 每条规则对应某通道的单一方向，作为规则大表下标。
 */
typedef enum
{
    DEV_MON_RULE_BUS_VOLT_UP, //!< 母线电压上限规则。
    DEV_MON_RULE_BUS_VOLT_DN, //!< 母线电压下限规则。
    DEV_MON_RULE_BUS_CUR_UP,  //!< 母线电流上限规则。
    DEV_MON_RULE_BUS_CUR_DN,  //!< 母线电流下限规则。
    DEV_MON_RULE_TEMP_UP,     //!< 温度上限规则。
    DEV_MON_RULE_TEMP_DN,     //!< 温度下限规则。

    DEV_MON_RULE_AMT,         //!< 规则数量。
} enDevMonRule;

/*****************************************************************************
 * 函数声明                                                                  *
 *****************************************************************************/
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化设备模拟量监控器。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 * @retval ER_SW_INV_PARAM 规则数量为 0 或存在通道号越界。
 */
err erInitDevMon(void);

/**
 * @brief 设备模拟量监控器周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 */
err erTckDevMon(void);

/**
 * @brief 获取单条规则当前是否故障。
 * @param[in] ku16Rule 规则下标。
 * @param[out] kpbFlt 故障态指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或故障态指针为空。
 * @retval ER_SW_INV_PARAM 规则下标越界。
 */
err eGetDevMonFlt(const u16 ku16Rule, bl* const kpbFlt);

/**
 * @brief 设置单通道采集值。
 * @details 由采集模块（如 ADC）在换算单位后调用，写入私有采集数组。
 * @param[in] eCh 通道枚举。
 * @param[in] s32Val 采集值（已换算单位）。
 * @return 设置结果。
 * @retval ER_SUC 设置成功。
 * @retval ER_SW_NUL_PTR 句柄或采集数组为空指针。
 * @retval ER_SW_INV_PARAM 通道号越界。
 */
err erSetDevMonVal(enDevMonCh eCh, s32 s32Val);

/**
 * @brief 获取单通道采集值。
 * @param[in] eCh 通道枚举。
 * @param[out] kps32Val 采集值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、采集数组或值指针为空。
 * @retval ER_SW_INV_PARAM 通道号越界。
 */
err eGetDevMonVal(enDevMonCh eCh, s32* const kps32Val);

#endif // DEV_MON_H
