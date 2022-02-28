/**
 * @file com.c
 * @brief 通用文件。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#include "hdr.h"

#ifdef COM_H

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn char* GetFileNm(const char* const cpPath)
 * @brief 获取全路径的文件名。
 * @details 通过查找最后一个匹配的\符号来截取文件名。
 * @param[in] cpPath 文件全路径指针。
 * @return 文件名字符串指针。
 * @attention 该函数只适用于Linux平台，Windows平台需要将 '/' 改为 '\\' 。
 */
char* GetFileNm(const char* const cpPath)
{
    char* pcFileNm = NULL;

    pcFileNm = strrchr(cpPath, '/');

    if(pcFileNm)
    {
        pcFileNm = pcFileNm + 1u;
    }
    else
    {
        pcFileNm = (char*)cpPath;
    }

    return pcFileNm;
}

/**
 * @fn bool CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
 * @brief 比较byte。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpbyDat1 数组1。
 * @param[in] cpbyDat2 数组2。
 * @param[in] cwAmt 要比较的byte数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bool CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt)
{
    bool bEq = FALSE;
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
 * @fn bool CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
 * @brief 比较word。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpwDat1 数组1。
 * @param[in] cpwDat2 数组2。
 * @param[in] cwAmt 要比较的word数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bool CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt)
{
    bool bEq = FALSE;
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
 * @fn bool CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
 * @brief 比较dword。将两个数组的元素进行一一比较，返回比较结果。
 * @param[in] cpdwDat1 数组1。
 * @param[in] cpdwDat2 数组2。
 * @param[in] cwAmt 要比较的dword数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 */
bool CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt)
{
    bool bEq = FALSE;
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

#endif //COM_H
