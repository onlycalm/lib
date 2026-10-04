/**
 * @file DevDtc.c
 * @brief DTC 业务配置模块实现。
 * @details 定义本项目 DTC 的配置表、快照缓冲区、时间源与快照捕获回调，
 *          并实例化 DTC 句柄，对外提供包装 API。本文件是各项目的差异化配置，
 *          随项目不同而修改。
 * @author Calm
 * @data 2026-10-04
 * @version v1.0.0
 * @attention 时间源与快照捕获均为占位示例，接入实际项目时需替换为真实实现。
 * @copyright Calm
 */

#include <stdio.h>
#include <string.h>
#include "Com.h"
#include "Dtc.h"
#include "DevDtc.h"

/* ===== 静态函数声明 ===== */
/* -- 时间源 -- */
static u64 u64GetSysTm(void);
/* -- 快照捕获 -- */
static void vidSnpDevDtc(u16 u16Id, u8* pu8Snp);
/* -- NVM 保存/恢复 -- */
static err erSaveDtcNvm(const stDtcRec* const kptRec, const u16 ku16Amt);
static err erLoadDtcNvm(stDtcRec* const ptRec, const u16 ku16Amt);

/* ===== 变量定义 ===== */
/* == 静态变量 == */
/* 模拟系统时间（ms），每次读取步进 1000ms。 */
static u64 s_u64SysTm = 0u;

/* 快照缓冲区：每个 DTC 一块。 */
static u8 s_au8Snp[DEV_DTC_AMT][sizeof(stDevDtcSnp)];

/* 模拟 NVM（Flash）存储区：保存全部 DTC 记录，模拟掉电不丢失。 */
static stDtcRec s_atDtcNvm[DEV_DTC_AMT];

/* DTC 配置表。 */
static const stDtcCfg s_katDtcCfg[] =
{
    [DEV_DTC_TEMP_HIGH] =
    {
        .u16Cd = u16PkgDtc(DTC_CAT_ANA, 0u, 1u, 0x15u), // 0x8115
        .pu8Snp = s_au8Snp[DEV_DTC_TEMP_HIGH],
        .u16SnpSz = (u16)sizeof(stDevDtcSnp),
    },
    [DEV_DTC_VOLT_LOW] =
    {
        .u16Cd = u16PkgDtc(DTC_CAT_ANA, 0u, 5u, 0x62u), // 0x8562
        .pu8Snp = s_au8Snp[DEV_DTC_VOLT_LOW],
        .u16SnpSz = (u16)sizeof(stDevDtcSnp),
    },
    [DEV_DTC_COMM_LOST] =
    {
        .u16Cd = u16PkgDtc(DTC_CAT_COMM, 0u, 1u, 0x00u), // 0xC100
        .pu8Snp = s_au8Snp[DEV_DTC_COMM_LOST],
        .u16SnpSz = (u16)sizeof(stDevDtcSnp),
    },
};

// 校验 DTC 枚举数量与配置表元素数量一致。
ER_ENUM_ASSERT(DEV_DTC_AMT ==
               (sizeof(s_katDtcCfg) / sizeof(s_katDtcCfg[0])));

/* DTC 运行时记录数组。 */
static stDtcRec s_atDtcRec[DEV_DTC_AMT];

/* DTC 管理器句柄。 */
static stDtc s_tDevDtc =
{
    .ptRec = s_atDtcRec,         // 运行时记录数组（状态/次数/时间/快照标志）。
    .pktCfg = s_katDtcCfg,       // 配置数组（编号/快照缓冲区指针/大小）。
    .u16Amt = (u16)DEV_DTC_AMT,  // DTC 数量。
    .u8CfmCyc = 2u,              // 确认阈值：连续失败循环数。置历史故障。
    .u8HealCyc = 2u,             // 治愈阈值：连续正常循环数。恢复历史故障。
    .pfu64GetTm = u64GetSysTm,   // 时间源回调（取故障发生时间）。
    .pfvidSnp = vidSnpDevDtc,    // 快照捕获回调（故障沿冻结现场数据）。
    .pferSave = erSaveDtcNvm,    // 保存回调（写模拟 NVM）。
    .pferLoad = erLoadDtcNvm,    // 恢复回调（读模拟 NVM）。
};

/* ===== 函数定义 ===== */
/* == 静态函数 == */
/* -- 时间源 -- */
/**
 * @brief 获取系统时间。
 * @details 模拟 1 秒步进的系统时间（ms），实际项目应接入 RTC 或系统 tick。
 * @return 当前系统时间（ms）。
 */
static u64 u64GetSysTm(void)
{
    s_u64SysTm += 1000u;

    return s_u64SysTm;
}

/* -- 快照捕获 -- */
/**
 * @brief 捕获 DTC 快照。
 * @details 冻结故障发生瞬间的现场数据。本示例填充占位数据，实际项目应读取
 *          真实的通道/温度/电压等信号。
 * @param[in] u16Id DTC 索引。
 * @param[out] pu8Snp 快照缓冲区指针。
 */
static void vidSnpDevDtc(u16 u16Id, u8* pu8Snp)
{
    static u16 s_u16SnpCnt = 0u;
    stDevDtcSnp* ptSnp = (stDevDtcSnp*)pu8Snp;

    s_u16SnpCnt++;
    ptSnp->u8Ch = (u8)u16Id;
    ptSnp->u16Temp = 800u + s_u16SnpCnt;   // 示例：每次捕获递增。
    ptSnp->u16Volt = 11500u + s_u16SnpCnt; // 示例：每次捕获递增。

    printf("捕获 DTC[%u] 快照（第 %u 次）。\n", (unsigned)u16Id,
           (unsigned)s_u16SnpCnt);
}

/* -- NVM 保存/恢复 -- */
/**
 * @brief 保存全部 DTC 记录到模拟 NVM。
 * @param[in] kptRec 记录数组指针。
 * @param[in] ku16Amt 记录数量。
 * @return 保存结果（0 表示成功）。
 */
static err erSaveDtcNvm(const stDtcRec* const kptRec, const u16 ku16Amt)
{
    memcpy(s_atDtcNvm, kptRec, ku16Amt * sizeof(stDtcRec));

    return 0u;
}

/**
 * @brief 从模拟 NVM 恢复全部 DTC 记录。
 * @param[out] ptRec 记录数组指针。
 * @param[in] ku16Amt 记录数量。
 * @return 恢复结果（0 表示成功）。
 */
static err erLoadDtcNvm(stDtcRec* const ptRec, const u16 ku16Amt)
{
    memcpy(ptRec, s_atDtcNvm, ku16Amt * sizeof(stDtcRec));

    return 0u;
}

/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化设备 DTC 管理器。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或配置数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 数量为 0。
 */
err erInitDevDtc(void)
{
    return erInitDtc(&s_tDevDtc);
}

/**
 * @brief 上报设备 DTC 的当前检测结果。
 * @param[in] eId DTC 枚举。
 * @param[in] bAct 当前检测结果（TRUE 故障 / FALSE 正常）。
 * @return 上报结果。
 * @retval ER_SUC 上报成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erReportDevDtc(enDevDtc eId, bl bAct)
{
    return erReportDtc(&s_tDevDtc, (u16)eId, bAct);
}

/**
 * @brief 结束设备 DTC 的当前操作循环并评估。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erEndCycDevDtc(void)
{
    return erEndCycDtc(&s_tDevDtc);
}

/**
 * @brief 开始设备 DTC 的一个新操作循环。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erNewCycDevDtc(void)
{
    return erNewCycDtc(&s_tDevDtc);
}

/**
 * @brief 清除单个设备 DTC。
 * @param[in] eId DTC 枚举。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erClrDevDtc(enDevDtc eId)
{
    return erClrDtc(&s_tDevDtc, (u16)eId);
}

/**
 * @brief 清除全部设备 DTC。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erClrAllDevDtc(void)
{
    return erClrAllDtc(&s_tDevDtc);
}

/**
 * @brief 获取单个设备 DTC 的状态字节。
 * @param[in] eId DTC 枚举。
 * @param[out] kptSts 状态字节指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或状态指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcSts(enDevDtc eId, unDtcSts* const kptSts)
{
    return eGetDtcSts(&s_tDevDtc, (u16)eId, kptSts);
}

/**
 * @brief 获取单个设备 DTC 的发生次数。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu32Cnt 发生次数指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或次数指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcCnt(enDevDtc eId, u32* const kpu32Cnt)
{
    return eGetDtcCnt(&s_tDevDtc, (u16)eId, kpu32Cnt);
}

/**
 * @brief 获取单个设备 DTC 的首次/最近发生时间。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu64FstTm 首次发生时间指针。
 * @param[out] kpu64LstTm 最近发生时间指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或时间指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcTm(enDevDtc eId, u64* const kpu64FstTm,
                 u64* const kpu64LstTm)
{
    return eGetDtcTm(&s_tDevDtc, (u16)eId, kpu64FstTm, kpu64LstTm);
}

/**
 * @brief 获取单个设备 DTC 的快照。
 * @param[in] eId DTC 枚举。
 * @param[out] kptSnp 快照指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或快照指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 * @retval ER_SW_NOT_SUP 该 DTC 未配置快照缓冲区。
 * @retval ER_DAT_EMPTY 快照尚未捕获。
 */
err eGetDevDtcSnp(enDevDtc eId, stDevDtcSnp* const kptSnp)
{
    return eGetDtcSnp(&s_tDevDtc, (u16)eId, kptSnp,
                      (u16)sizeof(stDevDtcSnp));
}

/**
 * @brief 获取单个设备 DTC 的标准编号。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu16Cd DTC 编号指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、配置数组或编号指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcCd(enDevDtc eId, u16* const kpu16Cd)
{
    return eGetDtcCd(&s_tDevDtc, (u16)eId, kpu16Cd);
}

/**
 * @brief 保存全部设备 DTC 记录到 NVM。
 * @return 保存结果。
 * @retval ER_SUC 保存成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置保存回调。
 */
err erSaveDevDtc(void)
{
    return erSaveDtc(&s_tDevDtc);
}

/**
 * @brief 从 NVM 恢复全部设备 DTC 记录。
 * @return 恢复结果。
 * @retval ER_SUC 恢复成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置恢复回调。
 */
err erLoadDevDtc(void)
{
    return erLoadDtc(&s_tDevDtc);
}
