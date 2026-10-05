/**
 * @file DevMon.c
 * @brief 模拟量监控配置模块实现。
 * @details 定义监控规则大表、采集数组、监控句柄及各规则的故障/恢复回调。
 *          本文件是各项目的差异化配置，随项目不同而修改。
 * @author Calm
 * @data 2026-10-05
 * @version v1.0.0
 * @attention 阈值、回差、周期与时间均为示例占位值，接入实际项目时需替换为真实值。
 * @copyright Calm
 */

#include <stdio.h>
#include "Com.h"
#include "Mon.h"
#include "DevMon.h"

/* ===== 静态函数声明 ===== */
/**
 * @brief 故障/恢复回调。
 * @details 示例仅打印事件；实际项目在此上报 DTC 或执行保护动作。
 * @param[in] u8Ch 通道号。
 * @param[in] eDir 方向。
 * @param[in] eEvt 事件。
 * @param[in] s32Val 触发事件时的采样值。
 */
static void vidDevMonEvt(u8 u8Ch, enMonDir eDir, enMonEvt eEvt, s32 s32Val);
static bl bDevMonAdcRdy(void);

/* ===== 变量定义 ===== */
/* == 全局变量 == */
/* 采集数组：私有，外部经 erSetDevMonVal 写入，下标即通道号。 */
static s32 s_atDevMonDat[DEV_MON_CH_AMT];

/* 规则大表：每条规则 = 通道 + 方向 + 阈值 + 回差 + 检测周期 + 确认/恢复时间 +
 * 回调。示例数值按 1000 倍定标：电压 52.000V/42.000V、电流 ±20.000A、
 * 温度 85.000℃/-40.000℃，回差 1.000 单位，时间单位为 tick 数/检测次数。 */
static const stMonRule s_katDevMonRule[] =
{
    [DEV_MON_RULE_BUS_VOLT_UP] =
    {
        .u8Ch = DEV_MON_CH_BUS_VOLT,
        .eDir = MON_DIR_UP,
        .s32Thr = 52000,
        .s32Hys = 1000,
        .u32DetPrd = 1u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
    [DEV_MON_RULE_BUS_VOLT_DN] =
    {
        .u8Ch = DEV_MON_CH_BUS_VOLT,
        .eDir = MON_DIR_DN,
        .s32Thr = 42000,
        .s32Hys = 1000,
        .u32DetPrd = 1u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
    [DEV_MON_RULE_BUS_CUR_UP] =
    {
        .u8Ch = DEV_MON_CH_BUS_CUR,
        .eDir = MON_DIR_UP,
        .s32Thr = 20000,
        .s32Hys = 1000,
        .u32DetPrd = 2u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
    [DEV_MON_RULE_BUS_CUR_DN] =
    {
        .u8Ch = DEV_MON_CH_BUS_CUR,
        .eDir = MON_DIR_DN,
        .s32Thr = -20000,
        .s32Hys = 1000,
        .u32DetPrd = 2u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
    [DEV_MON_RULE_TEMP_UP] =
    {
        .u8Ch = DEV_MON_CH_TEMP,
        .eDir = MON_DIR_UP,
        .s32Thr = 85000,
        .s32Hys = 1000,
        .u32DetPrd = 5u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
    [DEV_MON_RULE_TEMP_DN] =
    {
        .u8Ch = DEV_MON_CH_TEMP,
        .eDir = MON_DIR_DN,
        .s32Thr = -40000,
        .s32Hys = 1000,
        .u32DetPrd = 5u,
        .u32CfmTm = 3u,
        .u32RcvTm = 3u,
        .pfbEn = bDevMonAdcRdy,
        .pfvidEvt = vidDevMonEvt,
    },
};

// 校验规则枚举数量与规则表元素数量一致。
ER_ENUM_ASSERT(DEV_MON_RULE_AMT ==
               (sizeof(s_katDevMonRule) / sizeof(s_katDevMonRule[0])));

/* 运行时记录数组（与规则一一对应）。 */
static stMonRec s_atDevMonRec[DEV_MON_RULE_AMT];

/* 监控句柄。 */
static stMon s_tDevMon =
{
    .pktRule = s_katDevMonRule,
    .ptRec = s_atDevMonRec,
    .ps32Dat = s_atDevMonDat,
    .u16RuleAmt = DEV_MON_RULE_AMT,
    .u8ChAmt = DEV_MON_CH_AMT,
};

/* ===== 函数定义 ===== */
/* == 静态函数 == */
/**
 * @brief 故障/恢复回调。
 * @details 示例仅打印事件；实际项目在此上报 DTC 或执行保护动作。
 * @param[in] u8Ch 通道号。
 * @param[in] eDir 方向。
 * @param[in] eEvt 事件。
 * @param[in] s32Val 触发事件时的采样值。
 */
static void vidDevMonEvt(u8 u8Ch, enMonDir eDir, enMonEvt eEvt, s32 s32Val)
{
    // 示例：打印故障/恢复事件。
    printf("Mon evt: ch=%u dir=%s evt=%s val=%ld\n",
           (unsigned)u8Ch,
           (eDir == MON_DIR_UP) ? "UP" : "DN",
           (eEvt == MON_EVT_FLT) ? "FLT" : "RCV",
           (long)s32Val);
}

/**
 * @brief 使能前提回调。
 * @details 示例：返回全局 ADC 就绪标志，未就绪时该规则不检测、不报故障。
 * @return 使能结果。
 * @retval TRUE 已就绪，允许检测。
 * @retval FALSE 未就绪，视为无故障。
 */
static bl bDevMonAdcRdy(void)
{
    return TRUE;
}

/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化设备模拟量监控器。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 * @retval ER_SW_INV_PARAM 规则数量为 0 或存在通道号越界。
 */
err erInitDevMon(void)
{
    return erInitMon(&s_tDevMon);
}

/**
 * @brief 设备模拟量监控器周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 */
err erTckDevMon(void)
{
    return erTckMon(&s_tDevMon);
}

/**
 * @brief 获取单条规则当前是否故障。
 * @param[in] ku16Rule 规则下标。
 * @param[out] kpbFlt 故障态指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或故障态指针为空。
 * @retval ER_SW_INV_PARAM 规则下标越界。
 */
err eGetDevMonFlt(const u16 ku16Rule, bl* const kpbFlt)
{
    return eGetMonFlt(&s_tDevMon, ku16Rule, kpbFlt);
}

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
err erSetDevMonVal(enDevMonCh eCh, s32 s32Val)
{
    return erSetMonVal(&s_tDevMon, (u8)eCh, s32Val);
}

/**
 * @brief 获取单通道采集值。
 * @param[in] eCh 通道枚举。
 * @param[out] kps32Val 采集值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、采集数组或值指针为空。
 * @retval ER_SW_INV_PARAM 通道号越界。
 */
err eGetDevMonVal(enDevMonCh eCh, s32* const kps32Val)
{
    return eGetMonVal(&s_tDevMon, (u8)eCh, kps32Val);
}
