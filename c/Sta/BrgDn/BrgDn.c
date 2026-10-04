/**
 * @file BrgDn.c
 * @brief 下电启动配置模块实现。
 * @details 定义状态回调表及各状态的进入、运行、转换、退出回调函数。本文件是各项目的差异化配置，随项目不同而修改。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @attention 转移函数中的条件判断均为占位示例，接入实际项目时需替换为真实条件。
 * @copyright Calm
 */

#include <stdio.h>
#include "Com.h"
#include "Sta.h"
#include "BrgDn.h"

/* ===== 回调函数声明 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
static void vidEntCtlStpSta(void);
static void vidEntCtlStpWaitSta(void);
static void vidEntPeriStpSta(void);
static void vidEntStgWaitSta(void);
static void vidEntDoneSta(void);
/* -- 状态运行函数 -- */
static void vidRunCtlStpSta(void);
static void vidRunCtlStpWaitSta(void);
static void vidRunPeriStpSta(void);
static void vidRunStgWaitSta(void);
static void vidRunDoneSta(void);
/* -- 状态转移函数 -- */
static u8 u8TrfCtlStpSta(u8 u8CurSta);
static u8 u8TrfCtlStpWaitSta(u8 u8CurSta);
static u8 u8TrfPeriStpSta(u8 u8CurSta);
static u8 u8TrfStgWaitSta(u8 u8CurSta);
static u8 u8TrfDoneSta(u8 u8CurSta);
/* -- 状态退出函数 -- */
static void vidExCtlStpSta(void);
static void vidExCtlStpWaitSta(void);
static void vidExPeriStpSta(void);
static void vidExStgWaitSta(void);
static void vidExDoneSta(void);

/* ===== 变量定义 ===== */
/* == 全局变量 == */
/* 状态回调表。 */
// 下电启动为线性流程，各状态依次前进；某一步失败则停留该步，不再向下跳转。
// [CtlStp] -> [CtlStpWait] -> [PeriStp] -> [StgWait] -> [Done]
static const stStaCb s_katBrgDnCbTbl[] =
{
    [BRG_DN_CTL_STP]      = {vidEntCtlStpSta,     vidRunCtlStpSta,     u8TrfCtlStpSta,     vidExCtlStpSta},
    [BRG_DN_CTL_STP_WAIT] = {vidEntCtlStpWaitSta,  vidRunCtlStpWaitSta,  u8TrfCtlStpWaitSta,  vidExCtlStpWaitSta},
    [BRG_DN_PERI_STP]     = {vidEntPeriStpSta,     vidRunPeriStpSta,     u8TrfPeriStpSta,     vidExPeriStpSta},
    [BRG_DN_STG_WAIT]     = {vidEntStgWaitSta,     vidRunStgWaitSta,     u8TrfStgWaitSta,     vidExStgWaitSta},
    [BRG_DN_DONE]         = {vidEntDoneSta,        vidRunDoneSta,        u8TrfDoneSta,        vidExDoneSta},
};

// 校验状态枚举数量与回调表元素数量一致。
ER_ENUM_ASSERT(BRG_DN_AMT ==
               (sizeof(s_katBrgDnCbTbl) / sizeof(s_katBrgDnCbTbl[0])));

/* 状态机句柄。 */
static stSta s_tBrgDn = {
    .pktCbTbl = s_katBrgDnCbTbl,
    .u8CurSta = BRG_DN_CTL_STP,
    .u8StaAmt = BRG_DN_AMT,
};

/* ===== 函数定义 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
/**
 * @brief 进入停止被控设备状态的处理。
 */
static void vidEntCtlStpSta(void)
{
    // 进入停止被控设备状态的处理逻辑。
    printf("Entering Controlled Device Stop State.\n");
}

/**
 * @brief 进入等待被控设备完全停止状态的处理。
 */
static void vidEntCtlStpWaitSta(void)
{
    // 进入等待被控设备完全停止状态的处理逻辑。
    printf("Entering Controlled Device Stop Wait State.\n");
}

/**
 * @brief 进入停止操作外围设备状态的处理。
 */
static void vidEntPeriStpSta(void)
{
    // 进入停止操作外围设备状态的处理逻辑。
    printf("Entering Peripheral Device Stop State.\n");
}

/**
 * @brief 进入等待存储完成状态的处理。
 */
static void vidEntStgWaitSta(void)
{
    // 进入等待存储完成状态的处理逻辑。
    printf("Entering Storage Wait State.\n");
}

/**
 * @brief 进入关断完成状态的处理。
 */
static void vidEntDoneSta(void)
{
    // 进入关断完成状态的处理逻辑。
    printf("Entering Bring-Down Done State.\n");
}

/* -- 状态运行函数 -- */
/**
 * @brief 运行停止被控设备状态的处理。
 */
static void vidRunCtlStpSta(void)
{
    // 运行停止被控设备状态的处理逻辑。
    printf("Running Controlled Device Stop State.\n");
}

/**
 * @brief 运行等待被控设备完全停止状态的处理。
 */
static void vidRunCtlStpWaitSta(void)
{
    // 运行等待被控设备完全停止状态的处理逻辑。
    printf("Running Controlled Device Stop Wait State.\n");
}

/**
 * @brief 运行停止操作外围设备状态的处理。
 */
static void vidRunPeriStpSta(void)
{
    // 运行停止操作外围设备状态的处理逻辑。
    printf("Running Peripheral Device Stop State.\n");
}

/**
 * @brief 运行等待存储完成状态的处理。
 */
static void vidRunStgWaitSta(void)
{
    // 运行等待存储完成状态的处理逻辑。
    printf("Running Storage Wait State.\n");
}

/**
 * @brief 运行关断完成状态的处理。
 */
static void vidRunDoneSta(void)
{
    // 运行关断完成状态的处理逻辑。
    printf("Running Bring-Down Done State.\n");
}

/* -- 状态转移函数 -- */
/**
 * @brief 停止被控设备状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_DN_CTL_STP_WAIT 转换到等待被控设备完全停止状态。
 */
static u8 u8TrfCtlStpSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 被控设备停止指令下发完成。
    {
        printf("To BRG_DN_CTL_STP_WAIT\n");

        u8NxtSta = (u8)BRG_DN_CTL_STP_WAIT;
    }

    return u8NxtSta;
}

/**
 * @brief 等待被控设备完全停止状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_DN_PERI_STP 转换到停止操作外围设备状态。
 */
static u8 u8TrfCtlStpWaitSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 被控设备完全停止。
    {
        printf("To BRG_DN_PERI_STP\n");

        u8NxtSta = (u8)BRG_DN_PERI_STP;
    }

    return u8NxtSta;
}

/**
 * @brief 停止操作外围设备状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_DN_STG_WAIT 转换到等待存储完成状态。
 */
static u8 u8TrfPeriStpSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 外围设备停止操作完成，且已触发参数保存。
    {
        printf("To BRG_DN_STG_WAIT\n");

        u8NxtSta = (u8)BRG_DN_STG_WAIT;
    }

    return u8NxtSta;
}

/**
 * @brief 等待存储完成状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_DN_DONE 转换到关断完成状态。
 */
static u8 u8TrfStgWaitSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 存储写入完成。
    {
        printf("To BRG_DN_DONE\n");

        u8NxtSta = (u8)BRG_DN_DONE;
    }

    return u8NxtSta;
}

/**
 * @brief 关断完成状态的转换处理。
 * @details 完成状态为终态，不再发生转换。
 * @return 转换后的下一状态。
 * @retval BRG_DN_DONE 保持完成状态。
 */
static u8 u8TrfDoneSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    return u8NxtSta;
}

/* -- 状态退出函数 -- */
/**
 * @brief 退出停止被控设备状态的处理。
 */
static void vidExCtlStpSta(void)
{
    // 退出停止被控设备状态的处理逻辑。
    printf("Exiting Controlled Device Stop State.\n");
}

/**
 * @brief 退出等待被控设备完全停止状态的处理。
 */
static void vidExCtlStpWaitSta(void)
{
    // 退出等待被控设备完全停止状态的处理逻辑。
    printf("Exiting Controlled Device Stop Wait State.\n");
}

/**
 * @brief 退出停止操作外围设备状态的处理。
 */
static void vidExPeriStpSta(void)
{
    // 退出停止操作外围设备状态的处理逻辑。
    printf("Exiting Peripheral Device Stop State.\n");
}

/**
 * @brief 退出等待存储完成状态的处理。
 */
static void vidExStgWaitSta(void)
{
    // 退出等待存储完成状态的处理逻辑。
    printf("Exiting Storage Wait State.\n");
}

/**
 * @brief 退出关断完成状态的处理。
 */
static void vidExDoneSta(void)
{
    // 退出关断完成状态的处理逻辑。
    printf("Exiting Bring-Down Done State.\n");
}

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
err erInitBrgDn(void)
{
    return erInitSta(&s_tBrgDn);
}

/**
 * @brief 下电启动状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckBrgDn(void)
{
    return erTckSta(&s_tBrgDn);
}

/**
 * @brief 获取当前下电启动状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurBrgDn(enBrgDn* const kpeCurSta)
{
    return eGetCurSta(&s_tBrgDn, (u8*)kpeCurSta);
}
