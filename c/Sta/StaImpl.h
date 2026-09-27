/**
 * @file StaImpl.h
 * @brief 状态机配置模块。
 * @details 定义本项目的状态枚举。各项目的差异化状态在此定义，框架部分无需修改。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef STA_IMPL_H
#define STA_IMPL_H

/* ===== 枚举定义 ===== */
/**
 * @enum ESta
 * @brief 状态枚举。
 */
typedef enum
{
    STA_INIT,    //!< 初始化状态。
    STA_STBY,    //!< 待机状态。
    STA_NML_RDY, //!< 常态准备状态。
    STA_NML,     //!< 常状态。
    STA_FLT,     //!< 故障状态。
    STA_PRE_RST, //!< 预复位状态。
    STA_PRE_SLP, //!< 预睡眠状态。
    STA_RST,     //!< 复位状态。
    STA_SLP,     //!< 睡眠状态。

    STA_MAX, //!< 状态最大值。
} ESta;

#endif // STA_IMPL_H
