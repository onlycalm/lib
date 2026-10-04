/**
 * @file RgQue.c
 * @brief 环形队列框架实现。
 * @details 实现环形队列的初始化、入队、出队、窥看、查询与清空。
 *          框架不持有缓冲区，通过句柄绑定的缓冲区与参数驱动，支持多实例。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#include "RgQue.h"
#include "Typ.h"
#include <string.h>
#define ER_DOM     ER_DOM_LIB
#define ER_SUB_DOM ER_SUB_DOM_LIB_C
#define ER_MOD     ER_MOD_RGQUE
#include "Er.h"

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化环形队列。
 * @details 校验句柄参数并将读写索引复位为 0（清空队列）。
 * @param[in, out] kptQue 环形队列句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 缓冲区为空或容量小于 2。
 */
err erInitRgQue(stRgQue* const kptQue)
{
    err erRet = ER_SW_UNKN;

    if(kptQue == NULL)
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if((kptQue->pu8Buf == NULL) || (kptQue->u16Cap < 2u))
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        kptQue->u16RdIdx = 0u;
        kptQue->u16WrIdx = 0u;

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 入队若干字节。
 * @details 空间不足时整批拒绝，不写入任何字节（原子性）。
 * @param[in, out] kptQue 环形队列句柄。
 * @param[in] kpvDat 待写入数据指针。
 * @param[in] ku16Amt 待写入字节数。
 * @return 入队结果。
 * @retval ER_SUC 入队成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待写入字节数为 0。
 * @retval ER_DAT_FUL 剩余空间不足。
 */
err erPshRgQue(stRgQue* const kptQue, const void* const kpvDat,
               const u16 ku16Amt)
{
    u16 u16Wr = 0u;
    u16 u16First = 0u;
    err erRet = ER_SW_UNKN;

    if((kptQue == NULL) || (kpvDat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Amt == 0u)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else if(u16GetFreeRgQue(kptQue) < ku16Amt)
    {
        erRet = ER_DAT_FUL;
    }
    else
    {
        u16Wr = kptQue->u16WrIdx;

        if((u16Wr + ku16Amt) <= kptQue->u16Cap)
        {
            memcpy(kptQue->pu8Buf + u16Wr, kpvDat, ku16Amt);
        }
        else
        {
            u16First = kptQue->u16Cap - u16Wr;

            memcpy(kptQue->pu8Buf + u16Wr, kpvDat, u16First);
            memcpy(kptQue->pu8Buf, (const u8*)kpvDat + u16First,
                   ku16Amt - u16First);
        }

        kptQue->u16WrIdx = (u16)((u16Wr + ku16Amt) % kptQue->u16Cap);

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 出队若干字节。
 * @details 数据不足时整批拒绝，不读出任何字节（原子性）。
 * @param[in, out] kptQue 环形队列句柄。
 * @param[out] pvDat 读出数据指针。
 * @param[in] ku16Amt 待读出字节数。
 * @return 出队结果。
 * @retval ER_SUC 出队成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待读出字节数为 0。
 * @retval ER_DAT_EMPTY 数据不足。
 */
err erPopRgQue(stRgQue* const kptQue, void* const pvDat, const u16 ku16Amt)
{
    u16 u16Rd = 0u;
    u16 u16First = 0u;
    err erRet = ER_SW_UNKN;

    if((kptQue == NULL) || (pvDat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Amt == 0u)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else if(u16GetUsedRgQue(kptQue) < ku16Amt)
    {
        erRet = ER_DAT_EMPTY;
    }
    else
    {
        u16Rd = kptQue->u16RdIdx;

        if((u16Rd + ku16Amt) <= kptQue->u16Cap)
        {
            memcpy(pvDat, kptQue->pu8Buf + u16Rd, ku16Amt);
        }
        else
        {
            u16First = kptQue->u16Cap - u16Rd;

            memcpy(pvDat, kptQue->pu8Buf + u16Rd, u16First);
            memcpy((u8*)pvDat + u16First, kptQue->pu8Buf,
                   ku16Amt - u16First);
        }

        kptQue->u16RdIdx = (u16)((u16Rd + ku16Amt) % kptQue->u16Cap);

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 窥看队头若干字节（不出队）。
 * @param[in] kpktQue 环形队列句柄。
 * @param[out] pvDat 读出数据指针。
 * @param[in] ku16Amt 待窥看字节数。
 * @return 窥看结果。
 * @retval ER_SUC 窥看成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待窥看字节数为 0。
 * @retval ER_DAT_EMPTY 数据不足。
 */
err erPkRgQue(const stRgQue* const kpktQue, void* const pvDat,
              const u16 ku16Amt)
{
    u16 u16Rd = 0u;
    u16 u16First = 0u;
    err erRet = ER_SW_UNKN;

    if((kpktQue == NULL) || (pvDat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Amt == 0u)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else if(u16GetUsedRgQue(kpktQue) < ku16Amt)
    {
        erRet = ER_DAT_EMPTY;
    }
    else
    {
        u16Rd = kpktQue->u16RdIdx;

        if((u16Rd + ku16Amt) <= kpktQue->u16Cap)
        {
            memcpy(pvDat, kpktQue->pu8Buf + u16Rd, ku16Amt);
        }
        else
        {
            u16First = kpktQue->u16Cap - u16Rd;

            memcpy(pvDat, kpktQue->pu8Buf + u16Rd, u16First);
            memcpy((u8*)pvDat + u16First, kpktQue->pu8Buf,
                   ku16Amt - u16First);
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取已用字节数。
 * @param[in] kpktQue 环形队列句柄。
 * @return 已用字节数。
 * @retval 0 句柄为空或队列为空。
 */
u16 u16GetUsedRgQue(const stRgQue* const kpktQue)
{
    u16 u16Used = 0u;

    if(kpktQue != NULL)
    {
        u16Used =
            (u16)((kpktQue->u16WrIdx - kpktQue->u16RdIdx + kpktQue->u16Cap) %
                  kpktQue->u16Cap);
    }

    return u16Used;
}

/**
 * @brief 获取可用字节数。
 * @param[in] kpktQue 环形队列句柄。
 * @return 可用字节数。
 * @retval 0 句柄为空或队列已满。
 */
u16 u16GetFreeRgQue(const stRgQue* const kpktQue)
{
    u16 u16Free = 0u;

    if(kpktQue != NULL)
    {
        u16Free = (u16)(kpktQue->u16Cap - 1u - u16GetUsedRgQue(kpktQue));
    }

    return u16Free;
}

/**
 * @brief 判断队列是否为空。
 * @param[in] kpktQue 环形队列句柄。
 * @return 判断结果。
 * @retval TRUE 队列为空。
 * @retval FALSE 队列不为空或句柄为空。
 */
bl bEmptyRgQue(const stRgQue* const kpktQue)
{
    bl bEmpty = FALSE;

    if(kpktQue != NULL)
    {
        bEmpty = (kpktQue->u16RdIdx == kpktQue->u16WrIdx) ? TRUE : FALSE;
    }

    return bEmpty;
}

/**
 * @brief 判断队列是否已满。
 * @param[in] kpktQue 环形队列句柄。
 * @return 判断结果。
 * @retval TRUE 队列已满。
 * @retval FALSE 队列未满或句柄为空。
 */
bl bFullRgQue(const stRgQue* const kpktQue)
{
    bl bFull = FALSE;

    if(kpktQue != NULL)
    {
        bFull = (((kpktQue->u16WrIdx + 1u) % kpktQue->u16Cap) ==
                 kpktQue->u16RdIdx) ?
                    TRUE :
                    FALSE;
    }

    return bFull;
}

/**
 * @brief 清空环形队列。
 * @details 将读写索引复位为 0。
 * @param[in, out] kptQue 环形队列句柄。
 * @return 清空结果。
 * @retval ER_SUC 清空成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err erResetRgQue(stRgQue* const kptQue)
{
    err erRet = ER_SW_UNKN;

    if(kptQue == NULL)
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        kptQue->u16RdIdx = 0u;
        kptQue->u16WrIdx = 0u;

        erRet = ER_SUC;
    }

    return erRet;
}
