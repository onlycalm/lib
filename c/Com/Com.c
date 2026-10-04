/**
 * @file Com.c
 * @brief 通用文件。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include <string.h>
#include "Com.h"
#include "Log.h"
#include "Typ.h"
#define ER_DOM      ER_DOM_LIB
#define ER_SUB_DOM  ER_SUB_DOM_LIB_C
#define ER_MOD      ER_MOD_COM
#include "Er.h"

#ifdef COM_H

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn bl CmpU8(const u8* const kpku8Dat1, const u8* const kpku8Dat2, const u16 ku16Amt)
 * @brief 比较u8。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] kpku8Dat1 数组1。
 * @param[in] kpku8Dat2 数组2。
 * @param[in] ku16Amt 要比较的u8数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bl CmpU8(const u8* const kpku8Dat1, const u8* const kpku8Dat2, const u16 ku16Amt)
{
    bl bEq = FALSE;
    u16 u16i = 0u;

    for(u16i = 0u; u16i < ku16Amt; u16i++)
    {
        if(kpku8Dat1[u16i] != kpku8Dat2[u16i])
        {
            break;
        }
    }

    if(u16i == ku16Amt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn bl CmpU16(const u16* const kpku16Dat1, const u16* const kpku16Dat2, const u16 ku16Amt)
 * @brief 比较u16。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] kpku16Dat1 数组1。
 * @param[in] kpku16Dat2 数组2。
 * @param[in] ku16Amt 要比较的u16数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bl CmpU16(const u16* const kpku16Dat1, const u16* const kpku16Dat2, const u16 ku16Amt)
{
    bl bEq = FALSE;
    u16 u16i = 0u;

    for(u16i = 0u; u16i < ku16Amt; u16i++)
    {
        if(kpku16Dat1[u16i] != kpku16Dat2[u16i])
        {
            break;
        }
    }

    if(u16i == ku16Amt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn bl CmpU32(const u32* const kpku32Dat1, const u32* const kpku32Dat2, const u16 ku16Amt)
 * @brief 比较u32。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] kpku32Dat1 数组1。
 * @param[in] kpku32Dat2 数组2。
 * @param[in] ku16Amt 要比较的u32数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bl CmpU32(const u32* const kpku32Dat1, const u32* const kpku32Dat2, const u16 ku16Amt)
{
    bl bEq = FALSE;
    u16 u16i = 0u;

    for(u16i = 0u; u16i < ku16Amt; u16i++)
    {
        if(kpku32Dat1[u16i] != kpku32Dat2[u16i])
        {
            break;
        }
    }

    if(u16i == ku16Amt)
    {
        bEq = TRUE;
    }

    return bEq;
}

u16 u16CvtEndn(const u16 ku16Dat)
{
    return ((ku16Dat >> 8U) & 0x00FFU) | ((ku16Dat << 8U) & 0xFF00U);
}

u32 u32CvtEndn(const u32 ku32Dat)
{
    return ((ku32Dat >> 24U) & 0x000000FFU) |
           ((ku32Dat >> 8U) & 0x0000FF00U) |
           ((ku32Dat << 8U) & 0x00FF0000U) |
           ((ku32Dat << 24U) & 0xFF000000U);
}

err erIpToU32(const char* const kpkcIp, EEndn eEndn, u32* const kpu32Ip)
{
    LogTr("Enter erIpToU32 function.");

    u8 au8IpAdr[IP_V4_SZ] = {0u};
    u32 u32IpAdr = 0u;
    int asIpAdr[IP_V4_SZ] = {0u};
    err erRslt = ER_SW_UNKN;

    LogInf("kpkcIp = %s", kpkcIp);
    LogInf("eEndn = %d", eEndn);
    LogInf("kpu32Ip = %p", (void*)kpu32Ip);

    if((kpkcIp != NULL) && (eEndn < EndnMax) && (kpu32Ip != NULL))
    {
        if(sscanf(kpkcIp,
                  "%d.%d.%d.%d",
                  &asIpAdr[0u],
                  &asIpAdr[1u],
                  &asIpAdr[2u],
                  &asIpAdr[3u]) == IP_V4_SZ)
        {
            if((bInRng(asIpAdr[0u], IP_MIN, IP_MAX)) &&
               (bInRng(asIpAdr[1u], IP_MIN, IP_MAX)) &&
               (bInRng(asIpAdr[2u], IP_MIN, IP_MAX)) &&
               (bInRng(asIpAdr[3u], IP_MIN, IP_MAX)))
            {
                au8IpAdr[0u] = asIpAdr[0u];
                au8IpAdr[1u] = asIpAdr[1u];
                au8IpAdr[2u] = asIpAdr[2u];
                au8IpAdr[3u] = asIpAdr[3u];

                LogInf("au8IpAdr[0u] = %d", au8IpAdr[0u]);
                LogInf("au8IpAdr[1u] = %d", au8IpAdr[1u]);
                LogInf("au8IpAdr[2u] = %d", au8IpAdr[2u]);
                LogInf("au8IpAdr[3u] = %d", au8IpAdr[3u]);

                if(eEndn == EndnLe)
                {
                    *kpu32Ip = u32MrU32(au8IpAdr[0],
                                        au8IpAdr[1],
                                        au8IpAdr[2],
                                        au8IpAdr[3]);
                }
                else
                {
                    *kpu32Ip = u32MrU32(au8IpAdr[3],
                                        au8IpAdr[2],
                                        au8IpAdr[1],
                                        au8IpAdr[0]);
                }

                LogInf("*kpu32Ip = 0x%08X", *kpu32Ip);

                erRslt = ER_SUC;
            }
            else
            {
                erRslt = ER_SW_UNKN;
                LogErr("Data overflow.");
            }
        }
        else
        {
            erRslt = ER_SW_UNKN;
            LogErr("Invalid IP address format.");
        }
    }
    else
    {
        erRslt = ER_SW_UNKN;
        LogErr("Input parameter check failed.");
    }

    LogTr("Exit erIpToU32 function.");

    return erRslt;
}

#endif //COM_H
