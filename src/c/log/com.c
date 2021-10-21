/**
 * @file type.h
 * @brief 类型定义。
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
 * @fn pchar GetFileNm(pchar pcPath)
 * @brief 获取全路径的文件名。
 * @details 通过查找最后一个匹配的\符号来截取文件名。
 * @param [in] pcPath 文件全路径。
 * @return 文件名字符串首地址。
 * @note 无
 * @attention 该函数只适用于Linux平台，Windows平台需要将 '/' 改为 `\\` 。
 */
pchar GetFileNm(pchar pcPath)
{
    pchar pcFileNm = NULL;

    pcFileNm = strrchr(pcPath, '/');

    if(pcFileNm)
    {
        pcFileNm = pcFileNm + 1u;
    }
    else
    {
        pcFileNm = pcPath;
    }

    return pcFileNm;
}

/**
 * @fn dtc Delay10Us(word wUs)
 * @brief 延迟(10 * wUs)us。
 * @details 无
 * @param [in] wUs 微秒数。
 * @return dtc.
 * @note 该函数由用户实现。
 * @attention 无
 */
dtc Delay10Us(word wUs)
{
    dword wEnd;

    SysTick->LOAD = 0x00FFFFFFul;
    SysTick->VAL  = 0u;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

    while(wUs-- > 0u)
    {
        SysTick->VAL = 0u;

        wEnd = 0x01000000ul - SystemCoreClock/100000ul;
        while(SysTick->VAL > wEnd)
        {
            ;
        }
    }

    SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));

    return DTC_OK;
}

/**
 * @fn dtc Delay1Ms(word wMs)
 * @brief 延迟(1 * wMs)ms。
 * @details 无
 * @param [in] wMs 毫秒数。
 * @return dtc.
 * @note 该函数由用户实现。
 * @attention 无
 */
dtc Delay1Ms(word wMs)
{
    dword wEnd;

    SysTick->LOAD = 0x00FFFFFFul;
    SysTick->VAL  = 0ul;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;

    while(wMs-- > 0ul)
    {
        SysTick->VAL  = 0ul;
        wEnd = 0x01000000ul - SystemCoreClock/1000ul;
        while(SysTick->VAL > wEnd)
        {
            ;
        }
    }

    SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));

    return DTC_OK;
}

/**
 * @fn BOOL CmpByte(byte* pbyDat1, byte* pbyDat2, byte byAmt)
 * @brief 比较字节。将两个数组的元素进行一一比较，返回比较结果。
 * @details 无
 * @param [in] pbyDat1 数组1。
 * @param [in] pbyDat2 数组2。
 * @param [in] byAmt 要比较的字节数。
 * @return 比较结果。
 * @retval TRUE 相等。
 * @retval FALSE 不相等。
 * @note 无
 * @attention 无
 */
BOOL CmpByte(byte* pbyDat1, byte* pbyDat2, byte byAmt)
{
    byte byi = 0u;
    BOOL bEq = FALSE;

    for(byi = 0u; (byi < byAmt) && (pbyDat1[byi] == pbyDat2[byi]); byi++)
    {
    }

    if(byi == byAmt)
    {
        bEq = TRUE;
    }

    return bEq;
}

#endif //COM_H
