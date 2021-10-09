/**
 * @file sftiic.c
 * @brief 软件IIC模块。
 * @details None
 * @author Calm
 * @date 2021-06-15
 * @version v1.0.0
 * @copyright Calm
 */

#include "header.h"

#ifdef SFT_IIC_H
/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/

/*****************************************************************************
 *枚举定义                                                                   *
 *****************************************************************************/
/**
 * @enum IicAckTyp
 * @brief IIC Ack类型。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef enum IicAckTyp
{
    IicAckTypAck,  //!< 应答。
    IicAckTypNAck, //!< 非应答。
}EIicAckTyp;

/**
 * @enum IicRwTyp
 * @brief IIC 读写类型。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef enum IicRwTyp
{
    IicRwTypRd = 0x01u, //!< IIC读。
    IicRwTypWr = 0x00u  //!< IIC写。
}EIicRdWrTyp;

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//内敛函数。
static __forceinline void HdlIicWrByte(Byte byDat);
static __forceinline DWord HdlIicRdByte(Byte *pabyDat);
static __forceinline EIicAckTyp HdlIicWaitAck(void);

//=============================================================================
//静态函数。

//=============================================================================
//普通函数

/*****************************************************************************
 *变量定义                                                                   *
 *****************************************************************************/

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//内敛函数。

//=============================================================================
//静态函数
/**
 * @fn static void HdlIicWrByte(Byte byDat)
 * @brief 处理IIC 写动作，写1Byte数据。
 * @details 无
 * @param byDat 写入数据。
 * @return 故障码。
 * @note 无
 * @attention 无
 */
static void HdlIicWrByte(Byte byDat)
{
    Byte byi = 0u;

    for(byi = 0u; byi < 8u; byi++)
    {
        SetSclPol(LOW);
        DelayUs(SFT_IIC_DLY_HLF);
        SetSdaPol(byDat & 0x80u);
        byDat <<= 1u;
        DelayUs(SFT_IIC_DLY_HLF);
        SetSclPol(HIGH);
        DelayUs(SFT_IIC_DLY);
    }

    SetSclPol(LOW);
    SetSdaPol(LOW);
}

/**
 * @fn static DWord HdlIicRdByte(Byte *pabyDat)
 * @brief 处理IIC 读动作，读1Byte数据。
 * @details 无
 * @param [out] pabyDat 读1Byte数据。
 * @return 故障码。
 * @note 无
 * @attention 无
 */
static DWord HdlIicRdByte(Byte *pabyDat)
{
    Byte byi = 0u;
    DWord dwRtc = DTC_OK;

    SetSdaIn();

    *pabyDat = 0u;

    for(byi = 0u; byi < 8u; byi++)
    {
        SetSclPol(LOW);
        DelayUs(SFT_IIC_DLY);
        SetSclPol(HIGH);
        DelayUs(SFT_IIC_DLY_HLF);

        *pabyDat <<= 1u;

        if(GetSdaPol())
        {
            *pabyDat |= 0x01u;
        }

        DelayUs(SFT_IIC_DLY_HLF);
    }

    SetSdaPol(LOW);
    SetSdaOd();

    return dwRtc;
}

/**
 * @fn static EIicAckTyp HdlIicWaitAck(void)
 * @brief 处理IIC Ack或NAck动作。
 * @details 无
 * @param [out] peIicAckTyp Ack类型。
 * @return void
 * @note 无
 * @attention 无
 */
static EIicAckTyp HdlIicWaitAck(void)
{
    Word wWaitCnt = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;

    SetSdaPol(HIGH);
    DelayUs(SFT_IIC_DLY_HLF);
    SetSclPol(HIGH);
    DelayUs(SFT_IIC_DLY_HLF);

    SetSdaIn();

    while((wWaitCnt < SFT_IIC_WAIT_ACK_TM) && (eIicAckTyp == IicAckTypNAck))
    {
        if(GetSdaPol() == LOW)
        {
            eIicAckTyp = IicAckTypAck;
        }

        wWaitCnt++;
    }

    SetSdaPol(LOW);
    SetSdaOd();

    DelayUs(SFT_IIC_DLY_HLF);
    SetSclPol(LOW);
    DelayUs(SFT_IIC_DLY_HLF);
    SetSdaPol(HIGH);
    DelayUs(SFT_IIC_DLY_HLF);

    return eIicAckTyp;
}

//=============================================================================
//普通函数
/**
 * @fn DWord WrIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
 * @brief 主设备写IIC数据到从机。
 * @details 可控制字节数。
 * @param [in] bySlvAddr 从机地址。
 * @param [in] RegAddr 寄存器地址。
 * @param [in] byByteAmt 写入字节数。
 * @param [in] pabyDat 写入数据数组指针，字节按大端。
 * @return 故障检测码。
 * @note 7bit地址。
 * @attention 无
 */
DWord WrIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
{
    Byte byi = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    DWord dwDtc = DTC_OK;

    HdlIicStrt();
    HdlIicWrByte((bySlvAddr << 1u) | IicRwTypWr);
    eIicAckTyp = HdlIicWaitAck();

    if(eIicAckTyp == IicAckTypAck)
    {
        HdlIicWrByte(RegAddr);
        eIicAckTyp = HdlIicWaitAck();

        if(eIicAckTyp == IicAckTypAck)
        {
            do
            {
                HdlIicWrByte(pabyDat[byi]);
                byi++;
                eIicAckTyp = HdlIicWaitAck();
            }while((byi < byByteAmt) && (eIicAckTyp == IicAckTypAck));

            if(eIicAckTyp == IicAckTypAck)
            {
                HdlIicStp();
            }
            else
            {
                dwDtc = DTC_FAULT;
            }
        }
        else
        {
            dwDtc = DTC_FAULT;
        }
    }
    else
    {
        dwDtc = DTC_FAULT;
    }

    SetSdaPol(HIGH);
    SetSclPol(HIGH);

    return dwDtc;
}

/**
 * @fn DWord RdIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
 * @brief 主设备写IIC数据到从机。
 * @details 可控制字节数。
 * @param [in] bySlvAddr 从机地址。
 * @param [in] RegAddr 寄存器地址。
 * @param [in] byByteAmt 读取字节数。
 * @param [out] pabyDat 数据存储数组指针，字节按大端。
 * @return 故障检测码。
 * @note 7bit地址。
 * @attention 无
 */
DWord RdIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
{
    Byte byi = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    DWord dwDtc = DTC_OK;

    HdlIicStrt();
    HdlIicWrByte((bySlvAddr << 1u) | IicRwTypWr);
    eIicAckTyp = HdlIicWaitAck();

    if(eIicAckTyp == IicAckTypAck)
    {
        HdlIicWrByte(RegAddr);
        eIicAckTyp = HdlIicWaitAck();

        if(eIicAckTyp == IicAckTypAck)
        {
            HdlIicStrt();
            HdlIicWrByte((bySlvAddr << 1u) | IicRwTypRd);
            eIicAckTyp = HdlIicWaitAck();

            if(eIicAckTyp == IicAckTypAck)
            {
                HdlIicRdByte(&pabyDat[byi]);
                byi++;

                while((byi < byByteAmt) && (eIicAckTyp == IicAckTypAck))
                {
                    HdlIicAck();
                    HdlIicRdByte(&pabyDat[byi]);
                    byi++;
                }

                HdlIicNAck();
                HdlIicStp();
            }
            else
            {
                dwDtc = DTC_FAULT;
            }
        }
        else
        {
            dwDtc = DTC_FAULT;
        }
    }
    else
    {
        dwDtc = DTC_FAULT;
    }

    SetSdaPol(HIGH);
    SetSclPol(HIGH);

    return dwDtc;
}

/**
 * @fn DWord WrIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
 * @brief 主设备写IIC数据到从机，带故障3次重试。
 * @details 可控制字节数。
 * @param [in] bySlvAddr 从机地址。
 * @param [in] RegAddr 寄存器地址。
 * @param [in] byByteAmt 写入字节数。
 * @param [in] pabyDat 写入数据数组指针，字节按大端。
 * @return 故障检测码。
 * @note 7bit地址。
 * @attention 无
 */
DWord WrIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
{
    Byte byWrCnt = 0u;
    DWord dwWrIicDatDtc = DTC_OK;
    DWord dwDtc = DTC_OK;

    do
    {
        dwWrIicDatDtc = WrIicDat(bySlvAddr, RegAddr, byByteAmt, pabyDat);
        byWrCnt++;
    }while((dwWrIicDatDtc != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dwWrIicDatDtc != DTC_OK)
    {
        dwDtc = DTC_FAULT;
    }

    return dwDtc;
}

/**
 * @fn DWord RdIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
 * @brief 主设备写IIC数据到从机，带故障3次重试。
 * @details 可控制字节数。
 * @param [in] bySlvAddr 从机地址。
 * @param [in] RegAddr 寄存器地址。
 * @param [in] byByteAmt 读取字节数。
 * @param [out] pabyDat 数据存储数组指针，字节按大端。
 * @return 故障检测码。
 * @note 7bit地址。
 * @attention 无
 */
DWord RdIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
{
    Byte byWrCnt = 0u;
    DWord dwWrIicDatDtc = DTC_OK;
    DWord dwDtc = DTC_OK;

    do
    {
        dwWrIicDatDtc = RdIicDat(bySlvAddr, RegAddr, byByteAmt, pabyDat);
        byWrCnt++;
    }while((dwWrIicDatDtc != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dwWrIicDatDtc != DTC_OK)
    {
        dwDtc = DTC_FAULT;
    }

    return dwDtc;
}

/**
 * @fn DWord WrIicDatRcl(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
 * @brief 主设备写IIC数据到从机，带回读3次重试。
 * @details 可控制字节数。
 * @param [in] bySlvAddr 从机地址。
 * @param [in] RegAddr 寄存器地址。
 * @param [in] byByteAmt 写入字节数。
 * @param [in] pabyDat 写入数据数组指针，字节按大端。
 * @return 故障检测码。
 * @note 7bit地址。
 * @attention 无
 */
DWord WrIicDatRcl(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat)
{
    Byte byWrCnt = 0u;
    Bool bEq = FALSE;
    DWord dwWrIicDatDtcChk = DTC_OK;
    DWord dwRdIicDatDtcChk = DTC_OK;
    DWord dwDtc = DTC_OK;
    Byte* pabyRclDat = (Byte*)malloc(byByteAmt);

    do
    {
        dwWrIicDatDtcChk = WrIicDatChk(bySlvAddr, RegAddr, byByteAmt, pabyDat);
        dwRdIicDatDtcChk = RdIicDatChk(bySlvAddr, RegAddr, byByteAmt, pabyRclDat);

        if((dwWrIicDatDtcChk == DTC_OK) && (dwRdIicDatDtcChk == DTC_OK))
        {
            if(CmpByte(pabyDat, pabyRclDat, byByteAmt))
            {
                bEq = TRUE;
            }
        }

        byWrCnt++;
    }while((byWrCnt < SFT_IIC_RETRY) && (bEq == FALSE));

    if(bEq == FALSE)
    {
        dwDtc = DTC_FAULT;
    }

    free(pabyRclDat);

    return dwDtc;
}

#endif //SFT_IIC_H
