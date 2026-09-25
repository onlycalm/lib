/**
 * @file com.c
 * @brief 通用文件。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include <string.h>
#include "com.h"
#include "log.h"
#include "typ.h"
#include "err.h"

#ifdef COM_H

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn BOOL CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
 * @brief 比较byte。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpbyDat1 数组1。
 * @param[in] cpbyDat2 数组2。
 * @param[in] cwAmt 要比较的byte数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpbyDat1[wi] != cpbyDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn BOOL CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
 * @brief 比较word。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpwDat1 数组1。
 * @param[in] cpwDat2 数组2。
 * @param[in] cwAmt 要比较的word数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpwDat1[wi] != cpwDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

/**
 * @fn BOOL CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
 * @brief 比较dword。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpdwDat1 数组1。
 * @param[in] cpdwDat2 数组2。
 * @param[in] cwAmt 要比较的dword数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
BOOL CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
{
    BOOL bEq = FALSE;
    word wi = 0u;

    for(wi = 0u; wi < cwAmt; wi++)
    {
        if(cpdwDat1[wi] != cpdwDat2[wi])
        {
            break;
        }
    }

    if(wi == cwAmt)
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
    err erRslt = EC_NOK;

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

                erRslt = EC_OK;
            }
            else
            {
                erRslt = EC_NOK;
                LogErr("Data overflow.");
            }
        }
        else
        {
            erRslt = EC_NOK;
            LogErr("Invalid IP address format.");
        }
    }
    else
    {
        erRslt = EC_NOK;
        LogErr("Input parameter check failed.");
    }

    LogTr("Exit erIpToU32 function.");

    return erRslt;
}

#endif //COM_H
