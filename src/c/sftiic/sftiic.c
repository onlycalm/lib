/**
 * @file sftiic.c
 * @brief 软件IIC模块。
 * @details IIC主机。
 * @author Calm
 * @date 2021-06-15
 * @version v1.0.0
 * @copyright Calm
 */

#include "hdr.h"

#ifdef SFT_IIC_H

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//宏函数定义
#ifndef CtrlBr
#error Please implement CtrlBr.
/**
 * @def CtrlBr()
 * @brief 控制软件IIC的通讯波特率。
 * @details 通过延迟实现，延迟时间 = 1 / Freq / 2。
 * @param 无
 * @return void
 * @note 该宏函数由用户实现。
 */
#define CtrlBr() \
do               \
{                \
}while(0u)
#endif //CtrlBr

#ifndef SetSdaIn
#error Please implement SetSdaIn.
/**
 * @def SetSdaIn()
 * @brief 设置SDA为输入IO口。
 * @details 浮空输入。
 * @param void
 * @return void
 * @note 该函数由用户实现。
 */
#define SetSdaIn() \
do                 \
{                  \
}while(0u)
#endif //SetSdaIn

#ifndef SetSdaOd
#error Please implement SetSdaOd.
/**
 * @def SetSdaOd()
 * @brief 设置SDA为开漏输出IO口。
 * @details 开漏输出。
 * @param void
 * @return void
 * @note 该函数由用户实现。
 */
#define SetSdaOd() \
do                 \
{                  \
}while(0u)
#endif //SetSdaOd

#ifndef SetSdaPol
#error Please implement SetSdaPol.
/**
 * @def SetSdaPol(bPol)
 * @brief 设置SDA极性。
 * @details 无
 * @param bPol
 * @arg HIGH 高。
 * @arg LOW 低。
 * @return void
 * @note 该函数由用户实现。
 */
#define SetSdaPol(bPol) \
do                      \
{                       \
}while(0u)
#endif //SetSdaPol

#ifndef SetSclPol
#error Please implement SetSclPol.
/**
 * @def SetSclPol(bPol)
 * @brief 设置SCL极性。
 * @details 无
 * @param bPol
 * @arg HIGH 高。
 * @arg LOW 低。
 * @return void
 * @note 该函数由用户实现。
 */
#define SetSclPol(bPol) \
do                      \
{                       \
}while(0u)
#endif //SetSclPol

#ifndef GetSdaPol
#error Please implement GetSdaPol.
/**
 * @def GetSdaPol(void)
 * @brief 获取SDA极性。
 * @details 无
 * @param void
 * @return 极性。
 * @retval HIGH 高。
 * @retval LOW 低。
 * @note 该函数由用户实现。
 */
#define GetSdaPol() \
do                  \
{                   \
}while(0u)
#endif //GetSdaPol

/*****************************************************************************
 *枚举定义                                                                   *
 *****************************************************************************/
/**
 * @enum IicAckTyp
 * @brief IIC Ack类型。
 */
typedef enum IicAckTyp
{
    IicAckTypAck, //!<应答。
    IicAckTypNAck //!<非应答。
}EIicAckTyp;

/**
 * @enum IicRwTyp
 * @brief IIC 读写类型。
 */
typedef enum IicRwTyp
{
    IicRwTypRd = 0x01u, //!<IIC读。
    IicRwTypWr = 0x00u  //!<IIC写。
}EIicRdWrTyp;

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//静态函数
//-----------------------------------------------------------------------------
//内敛函数
/**
 * @fn STC_INLINE void HdlIicStrt(void)
 * @brief 处理IIC Start动作。
 * @param void
 * @return void
 */
STC_INLINE void HdlIicStrt(void)
{
    SetSdaPol(HIGH);
    SetSclPol(HIGH);
    CtrlBr();
    CtrlBr();
    SetSdaPol(LOW);
    CtrlBr();
    SetSclPol(LOW);
}

/**
 * @fn STC_INLINE void HdlIicStp(void)
 * @brief 处理IIC Stop动作。
 * @param void
 * @return void
 */
STC_INLINE void HdlIicStp(void)
{
    SetSclPol(LOW);
    SetSdaPol(LOW);
    CtrlBr();
    SetSclPol(HIGH);
    CtrlBr();
    SetSdaPol(HIGH);
    CtrlBr();
    CtrlBr();
}

/**
 * @fn STC_INLINE void HdlIicAck(void)
 * @brief 处理IIC Ack动作。
 * @param void
 * @return void
 */
STC_INLINE void HdlIicAck(void)
{
    SetSdaPol(LOW);
    SetSclPol(LOW);
    CtrlBr();
    CtrlBr();
    SetSclPol(HIGH);
    CtrlBr();
    CtrlBr();
    SetSclPol(LOW);
    CtrlBr();
    CtrlBr();
}

/**
 * @fn STC_INLINE void HdlIicNAck(void)
 * @brief 处理IIC NAck动作。
 * @param void
 * @return void
 */
STC_INLINE void HdlIicNAck(void)
{
    SetSclPol(LOW);
    SetSdaPol(HIGH);
    CtrlBr();
    CtrlBr();
    SetSclPol(HIGH);
    CtrlBr();
    CtrlBr();
    SetSclPol(LOW);
}

/**
 * @fn STC_INLINE void HdlIicWrBy(byte byDat)
 * @brief 处理IIC 写动作，写1byte数据。
 * @param[in] byDat 写入数据。
 * @return 故障码。
 */
STC_INLINE void HdlIicWrBy(byte byDat)
{
    byte byi = 0u;

    for(byi = 0u; byi < 8u; byi++)
    {
        SetSclPol(LOW);
        CtrlBr();
        SetSdaPol(byDat & 0x80u);
        byDat <<= 1u;
        CtrlBr();
        SetSclPol(HIGH);
        CtrlBr();
        CtrlBr();
    }

    SetSclPol(LOW);
    SetSdaPol(LOW);
}

/**
 * @fn STC_INLINE dtc HdlIicRdBy(byte* const cpabyDat)
 * @brief 处理IIC 读动作，读1byte数据。
 * @param[out] cpabyDat 读1byte数据。
 * @return 故障码。
 */
STC_INLINE dtc HdlIicRdBy(byte* const cpabyDat)
{
    byte byi = 0u;
    dtc dtcRtn = DTC_OK;

    SetSdaIn();

    *cpabyDat = 0u;

    for(byi = 0u; byi < 8u; byi++)
    {
        SetSclPol(LOW);
        CtrlBr();
        CtrlBr();
        SetSclPol(HIGH);
        CtrlBr();

        *cpabyDat <<= 1u;

        if(GetSdaPol())
        {
            *cpabyDat |= 0x01u;
        }

        CtrlBr();
    }

    SetSdaPol(LOW);
    SetSdaOd();

    return dtcRtn;
}

/**
 * @fn STC_INLINE EIicAckTyp HdlIicWaitAck(void)
 * @brief 处理IIC Ack或NAck动作。
 * @return void
 */
STC_INLINE EIicAckTyp HdlIicWaitAck(void)
{
    EIicAckTyp eIicAckTyp = IicAckTypNAck;

    SetSdaIn();
    CtrlBr();
    SetSclPol(HIGH);
    CtrlBr();

    if(GetSdaPol() == LOW)
    {
        eIicAckTyp = IicAckTypAck;
    }

    CtrlBr();
    SetSclPol(LOW);
    SetSdaPol(HIGH); //先置高，避免窄脉冲。
    SetSdaOd();

    return eIicAckTyp;
}

//=============================================================================
//普通函数
/**
 * @fn dtc WrIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
 * @brief IIC主设备写连续byte数据到从机。
 * @details 先发数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwByAmt 写入byte数。
 * @param[in] cpabyDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
{
    word wi = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrBy(cbyRegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            do
            {
                HdlIicWrBy(cpabyDat[wi++]);
                eIicAckTyp = HdlIicWaitAck();
            }while((eIicAckTyp == IicAckTypAck) && (wi < cwByAmt));

            if(eIicAckTyp == IicAckTypNAck)
            {
                dtcRtn = DTC_ERR;
            }
        }
        else
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    HdlIicStp();

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
 *                    const EEndn ceEndn, word* const cpawDat)
 * @brief IIC主设备写连续word数据到从机。
 * @details 先发数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwWdAmt 写入word数。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] cpawDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
               const EEndn ceEndn, word* const cpawDat)
{
    byte byLoBy = 0u;
    byte byHiBy = 0u;
    word wi = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrBy(cbyRegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            if(ceEndn == EndnLe)
            {
                do
                {
                    byLoBy = (byte)(cpawDat[wi] & 0x00FFu);
                    byHiBy = (byte)((cpawDat[wi] & 0xFF00u) >> 8u);

                    HdlIicWrBy(byLoBy);
                    eIicAckTyp = HdlIicWaitAck();

                    if(eIicAckTyp == IicAckTypAck)
                    {
                        HdlIicWrBy(byHiBy);
                        eIicAckTyp = HdlIicWaitAck();
                    }
                }while((eIicAckTyp == IicAckTypAck) && (++wi < cwWdAmt));
            }
            else if(ceEndn == EndnBe)
            {
                do
                {
                    byLoBy = (byte)(cpawDat[wi] & 0x00FFu);
                    byHiBy = (byte)((cpawDat[wi] & 0xFF00u) >> 8u);

                    HdlIicWrBy(byHiBy);
                    eIicAckTyp = HdlIicWaitAck();

                    if(eIicAckTyp == IicAckTypAck)
                    {
                        HdlIicWrBy(byLoBy);
                        eIicAckTyp = HdlIicWaitAck();
                    }
                }while((eIicAckTyp == IicAckTypAck) && (++wi < cwWdAmt));
            }

            if(eIicAckTyp == IicAckTypNAck)
            {
                dtcRtn = DTC_ERR;
            }
        }
        else
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    HdlIicStp();

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
 * @brief IIC主设备读从机连续byte数据。
 * @details 先读数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwByAmt 读取byte数。
 * @param[out] cpabyDat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
{
    word wi = 0u;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrBy(cbyRegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            HdlIicStrt();
            HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypRd);

            if(HdlIicWaitAck() == IicAckTypAck)
            {
                HdlIicRdBy(&cpabyDat[wi++]);

                while(wi < cwByAmt)
                {
                    HdlIicAck();
                    HdlIicRdBy(&cpabyDat[wi++]);
                }

                HdlIicNAck();
            }
            else
            {
                dtcRtn = DTC_ERR;
            }
        }
        else
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    HdlIicStp();

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
 *                    const EEndn ceEndn, word* const cpawDat)
 * @brief IIC主设备读从机连续word数据。
 * @details 先读数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwWdAmt 读取word数。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] cpawDat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
               const EEndn ceEndn, word* const cpawDat)
{
    byte byLoBy = 0u;
    byte byHiBy = 0u;
    word wi = 0u;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrBy(cbyRegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            HdlIicStrt();
            HdlIicWrBy((cbySlvAdr << 1u) | IicRwTypRd);

            if(HdlIicWaitAck() == IicAckTypAck)
            {
                if(ceEndn == EndnLe)
                {
                    HdlIicRdBy(&byLoBy);
                    HdlIicAck();
                    HdlIicRdBy(&byHiBy);
                    cpawDat[wi++] = (word)byLoBy | ((word)byHiBy << 8u);

                    while(wi < cwWdAmt)
                    {
                        HdlIicAck();
                        HdlIicRdBy(&byLoBy);
                        HdlIicAck();
                        HdlIicRdBy(&byHiBy);
                        cpawDat[wi++] = (word)byLoBy | ((word)byHiBy << 8u);
                    }
                }
                else if(ceEndn == EndnBe)
                {
                    HdlIicRdBy(&byHiBy);
                    HdlIicAck();
                    HdlIicRdBy(&byLoBy);
                    cpawDat[wi++] = (word)byLoBy | ((word)byHiBy << 8u);

                    while(wi < cwWdAmt)
                    {
                        HdlIicAck();
                        HdlIicRdBy(&byHiBy);
                        HdlIicAck();
                        HdlIicRdBy(&byLoBy);
                        cpawDat[wi++] = (word)byLoBy | ((word)byHiBy << 8u);
                    }
                }

                HdlIicNAck();
            }
            else
            {
                dtcRtn = DTC_ERR;
            }
        }
        else
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    HdlIicStp();

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerByChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
 * @brief IIC主设备写连续word数据到从机，带故障重试。
 * @details 先写数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwByAmt 写入byte数。
 * @param[in] cpabyDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerByChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
{
    byte byWrCnt = 0u;
    dtc dtcWrIicSerBy = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerBy = WrIicSerBy(cbySlvAdr, cbyRegAdr, cwByAmt, cpabyDat);
        byWrCnt++;
    }while((dtcWrIicSerBy != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerBy != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
 *                       const EEndn ceEndn, word* const cpawDat)
 * @brief IIC主设备写连续word数据到从机，带故障重试。
 * @details 先读数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwWdAmt 写入byte数。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] cpawDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                  const EEndn ceEndn, word* const cpawDat)
{
    byte byWrCnt = 0u;
    dtc dtcWrIicSerWd = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerWd = WrIicSerWd(cbySlvAdr, cbyRegAdr, cwWdAmt, ceEndn, cpawDat);
        byWrCnt++;
    }while((dtcWrIicSerWd != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerWd != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerByChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
 * @brief IIC主设备读从机连续byte数据，带故障重试。
 * @details 先读数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwByAmt 读取byte数。
 * @param[out] cpabyDat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerByChk(const byte cbySlvAdr, byte cbyRegAdr, const word cwByAmt, byte* cpabyDat)
{
    byte byWrCnt = 0u;
    dtc dtcWrIicSerBy = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerBy = RdIicSerBy(cbySlvAdr, cbyRegAdr, cwByAmt, cpabyDat);
        byWrCnt++;
    }while((dtcWrIicSerBy != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerBy != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
 *                       const EEndn ceEndn, word* const cpawDat)
 * @brief IIC主设备读从机连续word数据，带故障重试。
 * @details 先读数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwWdAmt 读取word数。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] cpawDat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                  const EEndn ceEndn, word* const cpawDat)
{
    byte byWrCnt = 0u;
    dtc dtcWrIicSerWd = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerWd = RdIicSerWd(cbySlvAdr, cbyRegAdr, cwWdAmt, ceEndn, cpawDat);
        byWrCnt++;
    }while((dtcWrIicSerWd != DTC_OK) && (byWrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerWd != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerByRcl(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
 * @brief IIC主设备写连续byte数据到从机，带回读重试。
 * @details 可控制byte数。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwByAmt 写入byte数。
 * @param[in] cpabyDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerByRcl(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt, byte* const cpabyDat)
{
    byte byWrCnt = 0u;
    bool bEq = FALSE;
    dtc dtcWrIicSerBy = DTC_OK;
    dtc dtcRdIicSerBy = DTC_OK;
    dtc dtcRtn = DTC_OK;
    byte* pabyRclDat = (byte*)malloc(cwByAmt * BY_SZ);

    do
    {
        dtcWrIicSerBy = WrIicSerByChk(cbySlvAdr, cbyRegAdr, cwByAmt, cpabyDat);
        dtcRdIicSerBy = RdIicSerByChk(cbySlvAdr, cbyRegAdr, cwByAmt, pabyRclDat);

        if((dtcWrIicSerBy == DTC_OK) && (dtcRdIicSerBy == DTC_OK))
        {
            if(CmpBy(cpabyDat, pabyRclDat, cwByAmt))
            {
                bEq = TRUE;
            }
        }

        byWrCnt++;
    }while((byWrCnt < SFT_IIC_RETRY) && (bEq == FALSE));

    if(bEq == FALSE)
    {
        dtcRtn = DTC_ERR;
    }

    free(pabyRclDat);

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerWdRcl(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
 *                       const EEndn ceEndn, word* const cpawDat)
 * @brief IIC主设备写连续byte数据到从机，带回读重试。
 * @details 先发数组小下标。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cwWdAmt 写入word数。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] cpawDat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerWdRcl(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                  const EEndn ceEndn, word* const cpawDat)
{
    byte byWrCnt = 0u;
    bool bEq = FALSE;
    dtc dtcWrIicSerWd = DTC_OK;
    dtc dtcRdIicSerWd = DTC_OK;
    dtc dtcRtn = DTC_OK;
    word* pawRclDat = (word*)malloc(cwWdAmt * WD_SZ);

    do
    {
        dtcWrIicSerWd = WrIicSerWdChk(cbySlvAdr, cbyRegAdr, cwWdAmt, ceEndn, cpawDat);
        dtcRdIicSerWd = RdIicSerWdChk(cbySlvAdr, cbyRegAdr, cwWdAmt, ceEndn, pawRclDat);

        if((dtcWrIicSerWd == DTC_OK) && (dtcRdIicSerWd == DTC_OK))
        {
            if(CmpWd(cpawDat, pawRclDat, cwWdAmt))
            {
                bEq = TRUE;
            }
        }

        byWrCnt++;
    }while((byWrCnt < SFT_IIC_RETRY) && (bEq == FALSE));

    if(bEq == FALSE)
    {
        dtcRtn = DTC_ERR;
    }

    free(pawRclDat);

    return dtcRtn;
}

/**
 * @fn dtc WrIicBy(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyDat)
 * @brief IIC主设备写byte数据到从机。
 * @details 写入byte数据。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cbyDat 写数据。
 * @return 故障检测码。
 */
dtc WrIicBy(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyDat)
{
    byte byDat = cbyDat;

    return WrIicSerByRcl(cbySlvAdr, cbyRegAdr, 1u, &byDat);
}

/**
 * @fn dtc WrIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, const word cwDat)
 * @brief IIC主设备写word数据到从机。
 * @details 区分字节序。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] cwDat 写数据。
 * @return 故障检测码。
 */
dtc WrIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, const word cwDat)
{
    word wDat = cwDat;

    return WrIicSerWdRcl(cbySlvAdr, cbyRegAdr, 1u, ceEndn, &wDat);
}

/**
 * @fn dtc RdIicBy(const byte cbySlvAdr, const byte cbyRegAdr, byte* const cpbyDat)
 * @brief IIC主设备读从机byte数据。
 * @details 读取byte数据。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[out] cpbyDat 读取数据。
 * @return 故障检测码。
 */
dtc RdIicBy(const byte cbySlvAdr, const byte cbyRegAdr, byte* const cpbyDat)
{
    return RdIicSerByChk(cbySlvAdr, cbyRegAdr, 1u, cpbyDat);
}

/**
 * @fn dtc RdIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, word* const cpwDat)
 * @brief IIC主设备读从机word数据。
 * @details 区分字节序。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] cpwDat 读取数据。
 * @return 故障检测码。
 */
dtc RdIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, word* const cpwDat)
{
    return RdIicSerWdChk(cbySlvAdr, cbyRegAdr, 1u, ceEndn, cpwDat);
}

/**
 * @fn dtc ModByBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMap, const byte cbyMd)
 * @brief 修改byte位。
 * @details 置位或复位寄存器位。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cbyMap 寄存器映射位。
 * @param[in] cbyMd 模式。
 * @arg RESET 复位映射位。
 * @arg SET 置位映射位。
 * @return 故障检测码。
 */
dtc ModByBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMap, const byte cbyMd)
{
    byte byTmp = 0u;
    dtc dtcRtn = DTC_OK;

    if(!RdIicBy(cbySlvAdr, cbyRegAdr, &byTmp))
    {
        byTmp = cbyMd ? SetMapBit(byTmp, cbyMap) : RstMapBit(byTmp, cbyMap);

        if(WrIicBy(cbySlvAdr, cbyRegAdr, byTmp))
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc ModWdBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMd, const EEndn ceEndn, const word cwMap)
 * @brief 修改word位。
 * @details 置位或复位寄存器位，区分字节序。
 * @param[in] cbySlvAdr 从机地址。
 * @param[in] cbyRegAdr 寄存器地址。
 * @param[in] cbyMd 模式。
 * @arg RESET 复位映射位。
 * @arg SET 置位映射位。
 * @param[in] ceEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] cwMap 寄存器映射位。
 * @return 故障检测码。
 */
dtc ModWdBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMd, const EEndn ceEndn, const word cwMap)
{
    word wTmp = 0u;
    dtc dtcRtn = DTC_OK;

    if(!RdIicWd(cbySlvAdr, cbyRegAdr, ceEndn, &wTmp))
    {
        wTmp = cbyMd ? SetMapBit(wTmp, cwMap) : RstMapBit(wTmp, cwMap);

        if(WrIicWd(cbySlvAdr, cbyRegAdr, ceEndn, wTmp))
        {
            dtcRtn = DTC_ERR;
        }
    }
    else
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

#endif //SFT_IIC_H
