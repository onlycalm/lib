/**
 * @file StaImpl.c
 * @brief 状态机配置模块实现。
 * @details 定义状态回调表及各状态的进入、运行、转换、退出回调函数。本文件是各项目的差异化配置，随项目不同而修改。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @attention 转移函数中的条件判断均为占位示例，接入实际项目时需替换为真实条件。
 * @copyright Calm
 */

#include <stdio.h>
#include "Sta.h"
#include "DevSta.h"

/* ===== 回调函数声明 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
static void vidEntInitSta(void);
static void vidEntStbySta(void);
static void vidEntNmlRdySta(void);
static void vidEntNmlSta(void);
static void vidEntFltSta(void);
static void vidEntPreRstSta(void);
static void vidEntPreSlpSta(void);
static void vidEntRstSta(void);
static void vidEntSlpSta(void);
/* -- 状态运行函数 -- */
static void vidRunInitSta(void);
static void vidRunStbySta(void);
static void vidRunNmlRdySta(void);
static void vidRunNmlSta(void);
static void vidRunFltSta(void);
static void vidRunPreRstSta(void);
static void vidRunPreSlpSta(void);
static void vidRunRstSta(void);
static void vidRunSlpSta(void);
/* -- 状态转移函数 -- */
static u8 u8TrfInitSta(u8 u8CurSta);
static u8 u8TrfStbySta(u8 u8CurSta);
static u8 u8TrfNmlRdySta(u8 u8CurSta);
static u8 u8TrfNmlSta(u8 u8CurSta);
static u8 u8TrfFltSta(u8 u8CurSta);
static u8 u8TrfPreRstSta(u8 u8CurSta);
static u8 u8TrfPreSlpSta(u8 u8CurSta);
static u8 u8TrfRstSta(u8 u8CurSta);
static u8 u8TrfSlpSta(u8 u8CurSta);
/* -- 状态退出函数 -- */
static void vidExInitSta(void);
static void vidExStbySta(void);
static void vidExNmlRdySta(void);
static void vidExNmlSta(void);
static void vidExFltSta(void);
static void vidExPreRstSta(void);
static void vidExPreSlpSta(void);
static void vidExRstSta(void);
static void vidExSlpSta(void);

/* ===== 变量定义 ===== */
/* == 全局变量 == */
/* 状态回调表。 */
// [Rst] <--------------复位请求-----------------_[Slp]
//  /|\  \                                       /|
//   |    \---复位---\                          /
//   |               _\|                       /
// [PreRst]_          [init]             _[PreSlp]
//  /|\   |\             |               /|
//   |      \        初始化完成         /
//   |       \           |             /
//   |      复位请求     |     休眠请求
//   |              \    |    /
//   |               \  \|/  /
//   |                [Stby] <------------------------|
//   |               /      \                         |
//   |              /       唤醒请求                  |
// 复位请求        /              _\/          待机、复位、休眠请求
//   |            /                [NmlRdy]           |
//   |   发生严重故障                      \          |
//   |   /                                 启动已稳定 |
//   | |/_                                        _\/ |
// [Flt] <--------------发生严重故障-------------- [Nml]
//       -----------------故障恢复--------------->
static const stStaCb s_katStaCbTbl[] =
{
    [DEV_STA_INIT]    = {vidEntInitSta,    vidRunInitSta,    u8TrfInitSta,    vidExInitSta},
    [DEV_STA_STBY]    = {vidEntStbySta,    vidRunStbySta,    u8TrfStbySta,    vidExStbySta},
    [DEV_STA_NML_RDY] = {vidEntNmlRdySta,  vidRunNmlRdySta,  u8TrfNmlRdySta,  vidExNmlRdySta},
    [DEV_STA_NML]     = {vidEntNmlSta,     vidRunNmlSta,     u8TrfNmlSta,     vidExNmlSta},
    [DEV_STA_FLT]     = {vidEntFltSta,     vidRunFltSta,     u8TrfFltSta,     vidExFltSta},
    [DEV_STA_PRE_RST] = {vidEntPreRstSta,  vidRunPreRstSta,  u8TrfPreRstSta,  vidExPreRstSta},
    [DEV_STA_PRE_SLP] = {vidEntPreSlpSta,  vidRunPreSlpSta,  u8TrfPreSlpSta,  vidExPreSlpSta},
    [DEV_STA_RST]     = {vidEntRstSta,     vidRunRstSta,     u8TrfRstSta,     vidExRstSta},
    [DEV_STA_SLP]     = {vidEntSlpSta,     vidRunSlpSta,     u8TrfSlpSta,     vidExSlpSta},
};

/* 状态机句柄。 */
static stSta s_tDevSta = {
    .pktCbTbl = s_katStaCbTbl,
    .u8CurSta = DEV_STA_INIT,
    .u8StaAmt = DEV_STA_AMT,
};

/* ===== 函数定义 ===== */
/* == 静态函数 == */
/* -- 状态进入函数 -- */
/**
 * @brief 进入初始化状态的处理。
 */
static void vidEntInitSta(void)
{
    // 进入初始化状态的处理逻辑。
    printf("Entering Initialization State.\n");
}

/**
 * @brief 进入待机状态的处理。
 */
static void vidEntStbySta(void)
{
    // 进入待机状态的处理逻辑。
    printf("Entering Standby State.\n");
}

/**
 * @brief 进入常态准备状态的处理。
 */
static void vidEntNmlRdySta(void)
{
    // 进入常态准备状态的处理逻辑。
    printf("Entering Normal Ready State.\n");
}

/**
 * @brief 进入常状态的处理。
 */
static void vidEntNmlSta(void)
{
    // 进入常状态的处理逻辑。
    printf("Entering Normal State.\n");
}

/**
 * @brief 进入故障状态的处理。
 */
static void vidEntFltSta(void)
{
    // 进入故障状态的处理逻辑。
    printf("Entering Fault State.\n");
}

/**
 * @brief 进入预复位状态的处理。
 */
static void vidEntPreRstSta(void)
{
    // 进入预复位状态的处理逻辑。
    printf("Entering Pre-Reset State.\n");
}

/**
 * @brief 进入预睡眠状态的处理。
 */
static void vidEntPreSlpSta(void)
{
    // 进入预睡眠状态的处理逻辑。
    printf("Entering Pre-Sleep State.\n");
}

/**
 * @brief 进入复位状态的处理。
 */
static void vidEntRstSta(void)
{
    // 进入复位状态的处理逻辑。
    printf("Entering Reset State.\n");
}

/**
 * @brief 进入睡眠状态的处理。
 */
static void vidEntSlpSta(void)
{
    // 进入睡眠状态的处理逻辑。
    printf("Entering Sleep State.\n");
}

/* -- 状态运行函数 -- */
/**
 * @brief 运行初始化状态的处理。
 */
static void vidRunInitSta(void)
{
    // 运行初始化状态的处理逻辑。
    printf("Running Initialization State.\n");
}

/**
 * @brief 运行待机状态的处理。
 */
static void vidRunStbySta(void)
{
    // 运行待机状态的处理逻辑。
    printf("Running Standby State.\n");
}

/**
 * @brief 运行常态准备状态的处理。
 */
static void vidRunNmlRdySta(void)
{
    // 运行常态准备状态的处理逻辑。
    printf("Running Normal Ready State.\n");
}

/**
 * @brief 运行常状态的处理。
 */
static void vidRunNmlSta(void)
{
    // 运行常状态的处理逻辑。
    printf("Running Normal State.\n");
}

/**
 * @brief 运行故障状态的处理。
 */
static void vidRunFltSta(void)
{
    // 运行故障状态的处理逻辑。
    printf("Running Fault State.\n");
}

/**
 * @brief 运行预复位状态的处理。
 */
static void vidRunPreRstSta(void)
{
    // 运行预复位状态的处理逻辑。
    printf("Running Pre-Reset State.\n");
}

/**
 * @brief 运行预睡眠状态的处理。
 */
static void vidRunPreSlpSta(void)
{
    // 运行预睡眠状态的处理逻辑。
    printf("Running Pre-Sleep State.\n");
}

/**
 * @brief 运行复位状态的处理。
 */
static void vidRunRstSta(void)
{
    // 运行复位状态的处理逻辑。
    printf("Running Reset State.\n");
}

/**
 * @brief 运行睡眠状态的处理。
 */
static void vidRunSlpSta(void)
{
    // 运行睡眠状态的处理逻辑。
    printf("Running Sleep State.\n");
}

/* -- 状态转移函数 -- */
/**
 * @brief 初始化状态的转换处理。
 * @return 转换后的下一状态。
 * @retval DEV_STA_STBY 转换到待机状态。
 */
static u8 u8TrfInitSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 初始化完成。
    {
        printf("To DEV_STA_STBY\n");

        u8NxtSta = (u8)DEV_STA_STBY;
    }

    return u8NxtSta;
}

/**
 * @brief 待机状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_NML_RDY 转换到常态准备状态。
 */
static u8 u8TrfStbySta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 收到复位请求。
    {
        printf("To DEV_STA_PRE_RST\n");

        u8NxtSta = (u8)DEV_STA_PRE_RST;
    }
    else if (1) // 收到休眠请求。
    {
        printf("To DEV_STA_PRE_SLP.\n");

        u8NxtSta = (u8)DEV_STA_PRE_SLP;
    }
    else if (1) // 发生故障。
    {
        printf("To DEV_STA_FLT.\n");

        u8NxtSta = (u8)DEV_STA_FLT;
    }
    else if (1) // 满足唤醒条件。
    {
        printf("To DEV_STA_NML_RDY\n");

        u8NxtSta = (u8)DEV_STA_NML_RDY;
    }


    return u8NxtSta;
}

/**
 * @brief 常态准备状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_NML 转换到常状态。
 */
static u8 u8TrfNmlRdySta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 设备启动达到稳定状态。
    {
        printf("To DEV_STA_NML\n");

        u8NxtSta = (u8)DEV_STA_NML;
    }

    return u8NxtSta;
}

/**
 * @brief 常状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_FLT 转换到故障状态。
 */
static u8 u8TrfNmlSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 收到复位、休眠、待机请求。
    {
        printf("To DEV_STA_STBY\n");

        u8NxtSta = (u8)DEV_STA_STBY;
    }
    else if (1) // 发生严重等级故障。
    {
        printf("To DEV_STA_FLT\n");

        u8NxtSta = (u8)DEV_STA_FLT;
    }

    return u8NxtSta;
}

/**
 * @brief 故障状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_PRE_RST 转换到预复位状态。
 */
static u8 u8TrfFltSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 故障连续复位次数小于N或收到复位请求。
    {
        printf("To DEV_STA_PRE_RST\n");

        u8NxtSta = (u8)DEV_STA_PRE_RST;
    }
    else if (1) // 收到休眠请求。
    {
        printf("To DEV_STA_PRE_SLP\n");

        u8NxtSta = (u8)DEV_STA_PRE_SLP;
    }
    else if (1) // 严重故障都恢复。
    {
        printf("To DEV_STA_NML\n");

        u8NxtSta = (u8)DEV_STA_NML;
    }

    return u8NxtSta;
}

/**
 * @brief 预复位状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_RST 转换到复位状态。
 */
static u8 u8TrfPreRstSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 复位准备动作完成，比如停激光和电机，Flash写完成等。
    {
        printf("To DEV_STA_RST\n");

        u8NxtSta = (u8)DEV_STA_RST;
    }

    return u8NxtSta;
}

/**
 * @brief 预睡眠状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_SLP 转换到睡眠状态。
 */
static u8 u8TrfPreSlpSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 复位准备动作完成，比如停激光和电机，Flash写完成等。
    {
        printf("To DEV_STA_SLP\n");

        u8NxtSta = (u8)DEV_STA_SLP;
    }

    return u8NxtSta;
}

/**
 * @brief 复位状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_STBY 转换到待机状态。
 */
static u8 u8TrfRstSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    return u8NxtSta;
}

/**
 * @brief 睡眠状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_STBY 转换到待机状态。
 */
static u8 u8TrfSlpSta(u8 u8CurSta)
{
    u8 u8NxtSta = u8CurSta;

    if (1) // 收到复位请求。
    {
        printf("To DEV_STA_RST\n");

        u8NxtSta = (u8)DEV_STA_RST;
    }

    return u8NxtSta;
}

/* -- 状态退出函数 -- */
/**
 * @brief 退出初始化状态的处理。
 */
static void vidExInitSta(void)
{
    // 退出初始化状态的处理逻辑。
    printf("Exiting Initialization State.\n");
}

/**
 * @brief 退出待机状态的处理。
 */
static void vidExStbySta(void)
{
    // 退出待机状态的处理逻辑。
    printf("Exiting Standby State.\n");
}

/**
 * @brief 退出常态准备状态的处理。
 */
static void vidExNmlRdySta(void)
{
    // 退出常态准备状态的处理逻辑。
    printf("Exiting Normal Ready State.\n");
}

/**
 * @brief 退出常状态的处理。
 */
static void vidExNmlSta(void)
{
    // 退出常状态的处理逻辑。
    printf("Exiting Normal State.\n");
}

/**
 * @brief 退出故障状态的处理。
 */
static void vidExFltSta(void)
{
    // 退出故障状态的处理逻辑。
    printf("Exiting Fault State.\n");
}

/**
 * @brief 退出预复位状态的处理。
 */
static void vidExPreRstSta(void)
{
    // 退出预复位状态的处理逻辑。
    printf("Exiting Pre-Reset State.\n");
}

/**
 * @brief 退出预睡眠状态的处理。
 */
static void vidExPreSlpSta(void)
{
    // 退出预睡眠状态的处理逻辑。
    printf("Exiting Pre-Sleep State.\n");
}

/**
 * @brief 退出复位状态的处理。
 */
static void vidExRstSta(void)
{
    // 退出复位状态的处理逻辑。
    printf("Exiting Reset State.\n");
}

/**
 * @brief 退出睡眠状态的处理。
 */
static void vidExSlpSta(void)
{
    // 退出睡眠状态的处理逻辑。
    printf("Exiting Sleep State.\n");
}

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
err erInitDevSta(void)
{
    return erInitSta(&s_tDevSta);
}

/**
 * @brief 设备状态机周期处理函数。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckDevSta(void)
{
    return erTckSta(&s_tDevSta);
}

/**
 * @brief 获取当前设备状态。
 * @param[out] kpeCurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurDevSta(enDevSta* const kpeCurSta)
{
    return eGetCurSta(&s_tDevSta, (u8*)kpeCurSta);
}
