/**
 * @file Sta.c
 * @brief 状态机框架实现。
 * @details 实现状态机的初始化、周期处理及当前状态查询。
 *          框架不包含具体状态，通过外部状态回调表 g_kstStaCb 驱动。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#include "Sta.h"
#include "err.h"

/* ===== 变量声明 ===== */
extern const stStaCb g_kstStaCb[]; //!< 状态回调表。

/* ===== 变量定义 ===== */
static ESta s_eCurSta = STA_INIT; //!< 当前状态变量。

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化状态机。
 * @details 调用初始状态的进入回调，使状态机进入初始状态。
 * @return 初始化结果。
 * @retval EC_OK 初始化成功。
 */
err erInitSta(void)
{
    // 进入初始状态。
    if (g_kstStaCb[s_eCurSta].pfvidEnt != NULL)
    {
        g_kstStaCb[s_eCurSta].pfvidEnt(); // 处理状态进入。
    }

    return EC_OK;
}

/**
 * @brief 状态机周期处理函数。
 * @details 每个调用周期内依次完成：查询当前状态的转换条件；若发生转换，则退出旧状态并进入新状态；最后运行当前状态。
 * @return 处理结果。
 * @retval EC_OK 处理成功。
 */
err erTckSta(void)
{
    ESta eNxtSta = s_eCurSta;

    // 状态转换。
    if (g_kstStaCb[s_eCurSta].pfvidTrf != NULL)
    {
        eNxtSta = g_kstStaCb[s_eCurSta].pfvidTrf();
    }

    // 发生状态转换。
    if (eNxtSta != s_eCurSta)
    {
        if (g_kstStaCb[s_eCurSta].pfvidEx != NULL)
        {
            g_kstStaCb[s_eCurSta].pfvidEx(); // 处理状态退出。

            if (g_kstStaCb[eNxtSta].pfvidEnt != NULL)
            {
                s_eCurSta = eNxtSta;
                g_kstStaCb[eNxtSta].pfvidEnt(); // 处理状态进入。
            }
        }
    }

    // 运行当前状态。
    if (g_kstStaCb[s_eCurSta].pfvidRun != NULL)
    {
        g_kstStaCb[s_eCurSta].pfvidRun();
    }

    return EC_OK;
}

/**
 * @brief 获取当前状态。
 * @return 当前状态枚举。
 */
ESta eGetCurSta(void)
{
    return s_eCurSta;
}
