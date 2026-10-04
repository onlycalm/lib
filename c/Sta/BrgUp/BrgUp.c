/**
 * @file BrgUp.c
 * @brief 上电启动配置模块实现。
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
#include "BrgUp.h"

/* ===== 回调函数声明 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
static void vidEntChpInitSta(void);
static void vidEntBrdInitSta(void);
static void vidEntVltChkSta(void);
static void vidEntTmpChkSta(void);
static void vidEntPeriStaChkSta(void);
static void vidEntCalLdSta(void);
static void vidEntCtlStaChkSta(void);
static void vidEntCtlStrtSta(void);
static void vidEntCtlStblSta(void);
static void vidEntDoneSta(void);
/* -- 状态运行函数 -- */
static void vidRunChpInitSta(void);
static void vidRunBrdInitSta(void);
static void vidRunVltChkSta(void);
static void vidRunTmpChkSta(void);
static void vidRunPeriStaChkSta(void);
static void vidRunCalLdSta(void);
static void vidRunCtlStaChkSta(void);
static void vidRunCtlStrtSta(void);
static void vidRunCtlStblSta(void);
static void vidRunDoneSta(void);
/* -- 状态转移函数 -- */
static u8 u8TrfChpInitSta(u8 u8CurSta);
static u8 u8TrfBrdInitSta(u8 u8CurSta);
static u8 u8TrfVltChkSta(u8 u8CurSta);
static u8 u8TrfTmpChkSta(u8 u8CurSta);
static u8 u8TrfPeriStaChkSta(u8 u8CurSta);
static u8 u8TrfCalLdSta(u8 u8CurSta);
static u8 u8TrfCtlStaChkSta(u8 u8CurSta);
static u8 u8TrfCtlStrtSta(u8 u8CurSta);
static u8 u8TrfCtlStblSta(u8 u8CurSta);
static u8 u8TrfDoneSta(u8 u8CurSta);
/* -- 状态退出函数 -- */
static void vidExChpInitSta(void);
static void vidExBrdInitSta(void);
static void vidExVltChkSta(void);
static void vidExTmpChkSta(void);
static void vidExPeriStaChkSta(void);
static void vidExCalLdSta(void);
static void vidExCtlStaChkSta(void);
static void vidExCtlStrtSta(void);
static void vidExCtlStblSta(void);
static void vidExDoneSta(void);

/* ===== 变量定义 ===== */
/* == 全局变量 == */
/* 状态回调表。 */
// 上电启动为线性流程，各状态依次前进；某一步失败则停留该步，不再向下跳转。
// [ChpInit] -> [BrdInit] -> [VltChk] -> [TmpChk] -> [PeriStaChk]
//    -> [CalLd] -> [CtlStaChk] -> [CtlStrt] -> [CtlStbl] -> [Done]
static const stStaCb s_katBrgUpCbTbl[] =
{
    [BRG_UP_CHP_INIT]     = {vidEntChpInitSta,     vidRunChpInitSta,     u8TrfChpInitSta,     vidExChpInitSta},
    [BRG_UP_BRD_INIT]     = {vidEntBrdInitSta,     vidRunBrdInitSta,     u8TrfBrdInitSta,     vidExBrdInitSta},
    [BRG_UP_VLT_CHK]      = {vidEntVltChkSta,      vidRunVltChkSta,      u8TrfVltChkSta,      vidExVltChkSta},
    [BRG_UP_TMP_CHK]      = {vidEntTmpChkSta,      vidRunTmpChkSta,      u8TrfTmpChkSta,      vidExTmpChkSta},
    [BRG_UP_PERI_STA_CHK] = {vidEntPeriStaChkSta,  vidRunPeriStaChkSta,  u8TrfPeriStaChkSta,  vidExPeriStaChkSta},
    [BRG_UP_CAL_LD]       = {vidEntCalLdSta,       vidRunCalLdSta,       u8TrfCalLdSta,       vidExCalLdSta},
    [BRG_UP_CTL_STA_CHK]  = {vidEntCtlStaChkSta,   vidRunCtlStaChkSta,   u8TrfCtlStaChkSta,   vidExCtlStaChkSta},
    [BRG_UP_CTL_STRT]     = {vidEntCtlStrtSta,     vidRunCtlStrtSta,     u8TrfCtlStrtSta,     vidExCtlStrtSta},
    [BRG_UP_CTL_STBL]     = {vidEntCtlStblSta,     vidRunCtlStblSta,     u8TrfCtlStblSta,     vidExCtlStblSta},
    [BRG_UP_DONE]         = {vidEntDoneSta,        vidRunDoneSta,        u8TrfDoneSta,        vidExDoneSta},
};

// 校验状态枚举数量与回调表元素数量一致。
ER_ENUM_ASSERT(BRG_UP_AMT ==
               (sizeof(s_katBrgUpCbTbl) / sizeof(s_katBrgUpCbTbl[0])));

/* 状态机句柄。 */
static stSta s_tBrgUp = {
    .pktCbTbl = s_katBrgUpCbTbl,
    .u8CurSta = BRG_UP_CHP_INIT,
    .u8StaAmt = BRG_UP_AMT,
};

/* ===== 函数定义 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
/**
 * @brief 进入片级初始化状态的处理。
 */
static void vidEntChpInitSta(void)
{
    // 进入片级初始化状态的处理逻辑。
    printf("Entering Chip Initialization State.\n");
}

/**
 * @brief 进入板级初始化状态的处理。
 */
static void vidEntBrdInitSta(void)
{
    // 进入板级初始化状态的处理逻辑。
    printf("Entering Board Initialization State.\n");
}

/**
 * @brief 进入电压检查状态的处理。
 */
static void vidEntVltChkSta(void)
{
    // 进入电压检查状态的处理逻辑。
    printf("Entering Voltage Check State.\n");
}

/**
 * @brief 进入温度检查状态的处理。
 */
static void vidEntTmpChkSta(void)
{
    // 进入温度检查状态的处理逻辑。
    printf("Entering Temperature Check State.\n");
}

/**
 * @brief 进入外围设备状态检查状态的处理。
 */
static void vidEntPeriStaChkSta(void)
{
    // 进入外围设备状态检查状态的处理逻辑。
    printf("Entering Peripheral Device Status Check State.\n");
}

/**
 * @brief 进入标定数据加载状态的处理。
 */
static void vidEntCalLdSta(void)
{
    // 进入标定数据加载状态的处理逻辑。
    printf("Entering Calibration Data Load State.\n");
}

/**
 * @brief 进入被控设备状态检查状态的处理。
 */
static void vidEntCtlStaChkSta(void)
{
    // 进入被控设备状态检查状态的处理逻辑。
    printf("Entering Controlled Device Status Check State.\n");
}

/**
 * @brief 进入启动被控设备状态的处理。
 */
static void vidEntCtlStrtSta(void)
{
    // 进入启动被控设备状态的处理逻辑。
    printf("Entering Controlled Device Start State.\n");
}

/**
 * @brief 进入等待被控设备稳定状态的处理。
 */
static void vidEntCtlStblSta(void)
{
    // 进入等待被控设备稳定状态的处理逻辑。
    printf("Entering Controlled Device Stable State.\n");
}

/**
 * @brief 进入上电启动完成状态的处理。
 */
static void vidEntDoneSta(void)
{
    // 进入上电启动完成状态的处理逻辑。
    printf("Entering Bring-Up Done State.\n");
}

/* -- 状态运行函数 -- */
/**
 * @brief 运行片级初始化状态的处理。
 */
static void vidRunChpInitSta(void)
{
    // 运行片级初始化状态的处理逻辑。
    printf("Running Chip Initialization State.\n");
}

/**
 * @brief 运行板级初始化状态的处理。
 */
static void vidRunBrdInitSta(void)
{
    // 运行板级初始化状态的处理逻辑。
    printf("Running Board Initialization State.\n");
}

/**
 * @brief 运行电压检查状态的处理。
 */
static void vidRunVltChkSta(void)
{
    // 运行电压检查状态的处理逻辑。
    printf("Running Voltage Check State.\n");
}

/**
 * @brief 运行温度检查状态的处理。
 */
static void vidRunTmpChkSta(void)
{
    // 运行温度检查状态的处理逻辑。
    printf("Running Temperature Check State.\n");
}

/**
 * @brief 运行外围设备状态检查状态的处理。
 */
static void vidRunPeriStaChkSta(void)
{
    // 运行外围设备状态检查状态的处理逻辑。
    printf("Running Peripheral Device Status Check State.\n");
}

/**
 * @brief 运行标定数据加载状态的处理。
 */
static void vidRunCalLdSta(void)
{
    // 运行标定数据加载状态的处理逻辑。
    printf("Running Calibration Data Load State.\n");
}

/**
 * @brief 运行被控设备状态检查状态的处理。
 */
static void vidRunCtlStaChkSta(void)
{
    // 运行被控设备状态检查状态的处理逻辑。
    printf("Running Controlled Device Status Check State.\n");
}

/**
 * @brief 运行启动被控设备状态的处理。
 */
static void vidRunCtlStrtSta(void)
{
    // 运行启动被控设备状态的处理逻辑。
    printf("Running Controlled Device Start State.\n");
}

/**
 * @brief 运行等待被控设备稳定状态的处理。
 */
static void vidRunCtlStblSta(void)
{
    // 运行等待被控设备稳定状态的处理逻辑。
    printf("Running Controlled Device Stable State.\n");
}

/**
 * @brief 运行上电启动完成状态的处理。
 */
static void vidRunDoneSta(void)
{
    // 运行上电启动完成状态的处理逻辑。
    printf("Running Bring-Up Done State.\n");
}

/* -- 状态转移函数 -- */
/**
 * @brief 片级初始化状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_BRD_INIT 转换到板级初始化状态。
 */
static u8 u8TrfChpInitSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 片级初始化成功。
    {
        printf("To BRG_UP_BRD_INIT\n");

        u8NxtSta = (u8)BRG_UP_BRD_INIT;
    }

    return u8NxtSta;
}

/**
 * @brief 板级初始化状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_VLT_CHK 转换到电压检查状态。
 */
static u8 u8TrfBrdInitSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 板级初始化成功。
    {
        printf("To BRG_UP_VLT_CHK\n");

        u8NxtSta = (u8)BRG_UP_VLT_CHK;
    }

    return u8NxtSta;
}

/**
 * @brief 电压检查状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_TMP_CHK 转换到温度检查状态。
 */
static u8 u8TrfVltChkSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 电压检查通过。
    {
        printf("To BRG_UP_TMP_CHK\n");

        u8NxtSta = (u8)BRG_UP_TMP_CHK;
    }

    return u8NxtSta;
}

/**
 * @brief 温度检查状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_PERI_STA_CHK 转换到外围设备状态检查状态。
 */
static u8 u8TrfTmpChkSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 温度检查通过。
    {
        printf("To BRG_UP_PERI_STA_CHK\n");

        u8NxtSta = (u8)BRG_UP_PERI_STA_CHK;
    }

    return u8NxtSta;
}

/**
 * @brief 外围设备状态检查状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_CAL_LD 转换到标定数据加载状态。
 */
static u8 u8TrfPeriStaChkSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 外围设备状态检查通过。
    {
        printf("To BRG_UP_CAL_LD\n");

        u8NxtSta = (u8)BRG_UP_CAL_LD;
    }

    return u8NxtSta;
}

/**
 * @brief 标定数据加载状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_CTL_STA_CHK 转换到被控设备状态检查状态。
 */
static u8 u8TrfCalLdSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 标定数据加载成功。
    {
        printf("To BRG_UP_CTL_STA_CHK\n");

        u8NxtSta = (u8)BRG_UP_CTL_STA_CHK;
    }

    return u8NxtSta;
}

/**
 * @brief 被控设备状态检查状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_CTL_STRT 转换到启动被控设备状态。
 */
static u8 u8TrfCtlStaChkSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 被控设备状态检查通过。
    {
        printf("To BRG_UP_CTL_STRT\n");

        u8NxtSta = (u8)BRG_UP_CTL_STRT;
    }

    return u8NxtSta;
}

/**
 * @brief 启动被控设备状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_CTL_STBL 转换到等待被控设备稳定状态。
 */
static u8 u8TrfCtlStrtSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 被控设备启动完成。
    {
        printf("To BRG_UP_CTL_STBL\n");

        u8NxtSta = (u8)BRG_UP_CTL_STBL;
    }

    return u8NxtSta;
}

/**
 * @brief 等待被控设备稳定状态的转换处理。
 * @return 转换后的下一状态。
 * @retval BRG_UP_DONE 转换到上电启动完成状态。
 */
static u8 u8TrfCtlStblSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 被控设备达到稳定状态。
    {
        printf("To BRG_UP_DONE\n");

        u8NxtSta = (u8)BRG_UP_DONE;
    }

    return u8NxtSta;
}

/**
 * @brief 上电启动完成状态的转换处理。
 * @details 完成状态为终态，不再发生转换。
 * @return 转换后的下一状态。
 * @retval BRG_UP_DONE 保持完成状态。
 */
static u8 u8TrfDoneSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    return u8NxtSta;
}

/* -- 状态退出函数 -- */
/**
 * @brief 退出片级初始化状态的处理。
 */
static void vidExChpInitSta(void)
{
    // 退出片级初始化状态的处理逻辑。
    printf("Exiting Chip Initialization State.\n");
}

/**
 * @brief 退出板级初始化状态的处理。
 */
static void vidExBrdInitSta(void)
{
    // 退出板级初始化状态的处理逻辑。
    printf("Exiting Board Initialization State.\n");
}

/**
 * @brief 退出电压检查状态的处理。
 */
static void vidExVltChkSta(void)
{
    // 退出电压检查状态的处理逻辑。
    printf("Exiting Voltage Check State.\n");
}

/**
 * @brief 退出温度检查状态的处理。
 */
static void vidExTmpChkSta(void)
{
    // 退出温度检查状态的处理逻辑。
    printf("Exiting Temperature Check State.\n");
}

/**
 * @brief 退出外围设备状态检查状态的处理。
 */
static void vidExPeriStaChkSta(void)
{
    // 退出外围设备状态检查状态的处理逻辑。
    printf("Exiting Peripheral Device Status Check State.\n");
}

/**
 * @brief 退出标定数据加载状态的处理。
 */
static void vidExCalLdSta(void)
{
    // 退出标定数据加载状态的处理逻辑。
    printf("Exiting Calibration Data Load State.\n");
}

/**
 * @brief 退出被控设备状态检查状态的处理。
 */
static void vidExCtlStaChkSta(void)
{
    // 退出被控设备状态检查状态的处理逻辑。
    printf("Exiting Controlled Device Status Check State.\n");
}

/**
 * @brief 退出启动被控设备状态的处理。
 */
static void vidExCtlStrtSta(void)
{
    // 退出启动被控设备状态的处理逻辑。
    printf("Exiting Controlled Device Start State.\n");
}

/**
 * @brief 退出等待被控设备稳定状态的处理。
 */
static void vidExCtlStblSta(void)
{
    // 退出等待被控设备稳定状态的处理逻辑。
    printf("Exiting Controlled Device Stable State.\n");
}

/**
 * @brief 退出上电启动完成状态的处理。
 */
static void vidExDoneSta(void)
{
    // 退出上电启动完成状态的处理逻辑。
    printf("Exiting Bring-Up Done State.\n");
}

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
err erInitBrgUp(void)
{
    return erInitSta(&s_tBrgUp);
}

/**
 * @brief 上电启动状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckBrgUp(void)
{
    return erTckSta(&s_tBrgUp);
}

/**
 * @brief 获取当前上电启动状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurBrgUp(enBrgUp* const kpeCurSta)
{
    return eGetCurSta(&s_tBrgUp, (u8*)kpeCurSta);
}
