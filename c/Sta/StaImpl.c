/**
 * @file StaImpl.c
 * @brief 状态机配置模块实现。
 * @details 定义状态回调表及各状态的进入、运行、转换、退出回调函数。本文件是各项目的差异化配置，随项目不同而修改。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include "Sta.h"
#include "StaImpl.h"

#ifdef STA_IMPL_H

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
static ESta eTrfInitSta(void);
static ESta eTrfStbySta(void);
static ESta eTrfNmlRdySta(void);
static ESta eTrfNmlSta(void);
static ESta eTrfFltSta(void);
static ESta eTrfPreRstSta(void);
static ESta eTrfPreSlpSta(void);
static ESta eTrfRstSta(void);
static ESta eTrfSlpSta(void);
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
/*
 * 状态回调表：表项顺序对应 ESta 枚举顺序，共 STA_MAX 项。
 * 由框架 Sta.c 通过 extern 引用。
 */
const stStaCb g_kstStaCb[] =
{
    {vidEntInitSta,    vidRunInitSta,    eTrfInitSta,    vidExInitSta},
    {vidEntStbySta,    vidRunStbySta,    eTrfStbySta,    vidExStbySta},
    {vidEntNmlRdySta,  vidRunNmlRdySta,  eTrfNmlRdySta,  vidExNmlRdySta},
    {vidEntNmlSta,     vidRunNmlSta,     eTrfNmlSta,     vidExNmlSta},
    {vidEntFltSta,     vidRunFltSta,     eTrfFltSta,     vidExFltSta},
    {vidEntPreRstSta,  vidRunPreRstSta,  eTrfPreRstSta,  vidExPreRstSta},
    {vidEntPreSlpSta,  vidRunPreSlpSta,  eTrfPreSlpSta,  vidExPreSlpSta},
    {vidEntRstSta,     vidRunRstSta,     eTrfRstSta,     vidExRstSta},
    {vidEntSlpSta,     vidRunSlpSta,     eTrfSlpSta,     vidExSlpSta},
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
 * @retval STA_STBY 转换到待机状态。
 */
static ESta eTrfInitSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 初始化完成。
    {
        printf("To STA_STBY\n");

        eNxtSta = STA_STBY;
    }

    return eNxtSta;
}

/**
 * @brief 待机状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_NML_RDY 转换到常态准备状态。
 */
static ESta eTrfStbySta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 收到复位请求。
    {
        printf("To STA_PRE_RST\n");

        eNxtSta = STA_PRE_RST;
    }
    else if (1) // 收到休眠请求。
    {
        printf("To STA_PRE_SLP.\n");

        eNxtSta = STA_PRE_SLP;
    }
    else if (1) // 发生故障。
    {
        printf("To STA_FLT.\n");

        eNxtSta = STA_FLT;
    }
    else if (1) // 满足唤醒条件。
    {
        printf("To STA_NML_RDY\n");

        eNxtSta = STA_NML_RDY;
    }


    return eNxtSta;
}

/**
 * @brief 常态准备状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_NML 转换到常状态。
 */
static ESta eTrfNmlRdySta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 设备启动达到稳定状态。
    {
        printf("To STA_NML\n");

        eNxtSta = STA_NML;
    }

    return eNxtSta;
}

/**
 * @brief 常状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_FLT 转换到故障状态。
 */
static ESta eTrfNmlSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 收到复位、休眠、待机请求。
    {
        printf("To STA_STBY\n");

        eNxtSta = STA_STBY;
    }
    else if (1) // 发生严重等级故障。
    {
        printf("To STA_FLT\n");

        eNxtSta = STA_FLT;
    }

    return eNxtSta;
}

/**
 * @brief 故障状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_PRE_RST 转换到预复位状态。
 */
static ESta eTrfFltSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 故障连续复位次数小于N或收到复位请求。
    {
        printf("To STA_PRE_RST\n");

        eNxtSta = STA_PRE_RST;
    }
    else if (1) // 收到休眠请求。
    {
        printf("To STA_PRE_SLP\n");

        eNxtSta = STA_PRE_SLP;
    }
    else if (1) // 严重故障都恢复。
    {
        printf("To STA_NML\n");

        eNxtSta = STA_NML;
    }

    return eNxtSta;
}

/**
 * @brief 预复位状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_RST 转换到复位状态。
 */
static ESta eTrfPreRstSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 复位准备动作完成，比如停激光和电机，Flash写完成等。
    {
        printf("To STA_RST\n");

        eNxtSta = STA_RST;
    }

    return eNxtSta;
}

/**
 * @brief 预睡眠状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_SLP 转换到睡眠状态。
 */
static ESta eTrfPreSlpSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 复位准备动作完成，比如停激光和电机，Flash写完成等。
    {
        printf("To STA_SLP\n");

        eNxtSta = STA_SLP;
    }

    return eNxtSta;
}

/**
 * @brief 复位状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_STBY 转换到待机状态。
 */
static ESta eTrfRstSta(void)
{
    ESta eNxtSta = eGetCurSta();

    return eNxtSta;
}

/**
 * @brief 睡眠状态的转换处理。
 * @return 转换后的下一状态。
 * @retval STA_STBY 转换到待机状态。
 */
static ESta eTrfSlpSta(void)
{
    ESta eNxtSta = eGetCurSta();

    if (1) // 收到复位请求。
    {
        printf("To STA_RST\n");

        eNxtSta = STA_RST;
    }

    return eNxtSta;
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

#endif // STA_IMPL_H
