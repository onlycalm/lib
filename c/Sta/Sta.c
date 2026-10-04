/**
 * @file Sta.c
 * @brief 状态机框架实现。
 * @details 实现状态机的初始化、周期处理及当前状态查询。
 *          框架不包含具体状态，通过句柄绑定的状态回调表驱动，支持多实例。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#include "Sta.h"
#include "Typ.h"
#define ER_DOM      ER_DOM_LIB
#define ER_SUB_DOM  ER_SUB_DOM_LIB_C
#define ER_MOD      ER_MOD_STA
#include "Er.h"

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化状态机。
 * @details 绑定回调表与状态个数，将状态复位为初始状态（枚举值 0），
 *          随后调用初始状态的进入回调。
 * @param[in] kpktSta 状态机句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或回调表为空指针。
 * @retval ER_SW_INV_PARAM 状态个数为 0 或初始状态枚举值越界。
 */
err erInitSta(const stSta* const kpktSta)
{
    err erRet = ER_SW_UNKN;

    // 检查参数合法性。
    if ((kpktSta == NULL) || (kpktSta->pktCbTbl == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if ((kpktSta->u8StaAmt == 0u) ||
             (kpktSta->u8CurSta >= kpktSta->u8StaAmt))
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        if (kpktSta->pktCbTbl[kpktSta->u8CurSta].pfvidEnt != NULL)
        {
            kpktSta->pktCbTbl[kpktSta->u8CurSta].pfvidEnt(); // 处理状态进入。
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 状态机周期处理函数。
 * @details 每个调用周期内依次完成：查询当前状态的转换条件；若发生转换，
 *          则退出旧状态并进入新状态；最后运行当前状态。
 * @param[in, out] kptSta 状态机句柄。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckSta(stSta* const kptSta)
{
    u8 u8NxtSta = 0u;
    err erRet = ER_SW_UNKN;

    if ((kptSta == NULL) || (kptSta->pktCbTbl == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        erRet = ER_SUC;
        u8NxtSta = kptSta->u8CurSta;

        // 判断状态转换。
        if (kptSta->pktCbTbl[kptSta->u8CurSta].pfvidTrf != NULL)
        {
            u8NxtSta = kptSta->pktCbTbl[kptSta->u8CurSta].pfvidTrf(kptSta->u8CurSta);

            if (u8NxtSta >= kptSta->u8StaAmt)
            {
                u8NxtSta = kptSta->u8CurSta; // 越界值不切换状态。
                erRet = ER_SW_INV_PARAM;
            }
        }

        // 发生状态转换。
        if (u8NxtSta != kptSta->u8CurSta)
        {
            if (kptSta->pktCbTbl[kptSta->u8CurSta].pfvidEx != NULL)
            {
                kptSta->pktCbTbl[kptSta->u8CurSta].pfvidEx(); // 处理状态退出。
            }

            kptSta->u8CurSta = u8NxtSta; // 状态转移取决于转移函数，不一定有退出和进入函数。

            if (kptSta->pktCbTbl[kptSta->u8CurSta].pfvidEnt != NULL)
            {
                kptSta->pktCbTbl[kptSta->u8CurSta].pfvidEnt(); // 处理状态进入。
            }
        }

        // 运行当前状态。
        if (kptSta->pktCbTbl[kptSta->u8CurSta].pfvidRun != NULL)
        {
            kptSta->pktCbTbl[kptSta->u8CurSta].pfvidRun();
        }
    }

    return erRet;
}

/**
 * @brief 获取当前状态。
 * @param[in] kpktSta 状态机句柄。
 * @param[out] kpu8CurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurSta(const stSta* const kpktSta, u8* const kpu8CurSta)
{
    err erRtn = ER_SW_UNKN;

    if ((kpktSta == NULL) || (kpu8CurSta == NULL))
    {
        erRtn = ER_SW_NUL_PTR;
    }
    else
    {
        *kpu8CurSta = kpktSta->u8CurSta;
        erRtn = ER_SUC;
    }

    return erRtn;
}
