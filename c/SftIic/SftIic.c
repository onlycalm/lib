/**
 * @file SftIic.c
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
 * @fn STC_FRC_INLINE void HdlIicStrt(void)
 * @brief 处理IIC Start动作。
 * @param void
 * @return void
 */
STC_FRC_INLINE void HdlIicStrt(void)
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
 * @fn STC_FRC_INLINE void HdlIicStp(void)
 * @brief 处理IIC Stop动作。
 * @param void
 * @return void
 */
STC_FRC_INLINE void HdlIicStp(void)
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
 * @fn STC_FRC_INLINE void HdlIicAck(void)
 * @brief 处理IIC Ack动作。
 * @param void
 * @return void
 */
STC_FRC_INLINE void HdlIicAck(void)
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
 * @fn STC_FRC_INLINE void HdlIicNAck(void)
 * @brief 处理IIC NAck动作。
 * @param void
 * @return void
 */
STC_FRC_INLINE void HdlIicNAck(void)
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
 * @fn STC_FRC_INLINE void HdlIicWrU8(u8 u8Dat)
 * @brief 处理IIC 写动作，写1个u8数据。
 * @param[in] u8Dat 写入数据。
 * @return 故障码。
 */
STC_FRC_INLINE void HdlIicWrU8(u8 u8Dat)
{
    u8 u8i = 0u;

    for(u8i = 0u; u8i < 8u; u8i++)
    {
        SetSclPol(LOW);
        CtrlBr();
        SetSdaPol(u8Dat & 0x80u);
        u8Dat <<= 1u;
        CtrlBr();
        SetSclPol(HIGH);
        CtrlBr();
        CtrlBr();
    }

    SetSclPol(LOW);
    SetSdaPol(LOW);
}

/**
 * @fn STC_FRC_INLINE dtc HdlIicRdU8(u8* const kpu8Dat)
 * @brief 处理IIC 读动作，读1个u8数据。
 * @param[out] kpu8Dat 读1个u8数据。
 * @return 故障码。
 */
STC_FRC_INLINE dtc HdlIicRdU8(u8* const kpu8Dat)
{
    u8 u8i = 0u;
    dtc dtcRtn = DTC_OK;

    SetSdaIn();

    *kpu8Dat = 0u;

    for(u8i = 0u; u8i < 8u; u8i++)
    {
        SetSclPol(LOW);
        CtrlBr();
        CtrlBr();
        SetSclPol(HIGH);
        CtrlBr();

        *kpu8Dat <<= 1u;

        if(GetSdaPol())
        {
            *kpu8Dat |= 0x01u;
        }

        CtrlBr();
    }

    SetSdaPol(LOW);
    SetSdaOd();

    return dtcRtn;
}

/**
 * @fn STC_FRC_INLINE EIicAckTyp HdlIicWaitAck(void)
 * @brief 处理IIC Ack或NAck动作。
 * @param void
 * @return Ack类型。
 */
STC_FRC_INLINE EIicAckTyp HdlIicWaitAck(void)
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
 * @fn dtc WrIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
 *                    const u8* const kpku8Dat)
 * @brief IIC主设备写连续u8数据到从机。
 * @details 先发数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U8Amt 写入u8数。
 * @param[in] kpku8Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, const u8* const kpku8Dat)
{
    u16 u16i = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrU8(ku8RegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            do
            {
                HdlIicWrU8(kpku8Dat[u16i++]);
                eIicAckTyp = HdlIicWaitAck();
            }while((eIicAckTyp == IicAckTypAck) && (u16i < ku16U8Amt));

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
 * @fn dtc WrIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
 *                    const EEndn keEndn, const u16* const kpku16Dat)
 * @brief IIC主设备写连续u16数据到从机。
 * @details 先发数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U16Amt 写入u16数。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] kpku16Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
               const EEndn keEndn, const u16* const kpku16Dat)
{
    u8 u8LoU8 = 0u;
    u8 u8HiU8 = 0u;
    u16 u16i = 0u;
    EIicAckTyp eIicAckTyp = IicAckTypNAck;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrU8(ku8RegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            if(keEndn == EndnLe)
            {
                do
                {
                    u8LoU8 = (u8)(kpku16Dat[u16i] & 0x00FFu);
                    u8HiU8 = (u8)((kpku16Dat[u16i] & 0xFF00u) >> 8u);

                    HdlIicWrU8(u8LoU8);
                    eIicAckTyp = HdlIicWaitAck();

                    if(eIicAckTyp == IicAckTypAck)
                    {
                        HdlIicWrU8(u8HiU8);
                        eIicAckTyp = HdlIicWaitAck();
                    }
                }while((eIicAckTyp == IicAckTypAck) && (++u16i < ku16U16Amt));
            }
            else if(keEndn == EndnBe)
            {
                do
                {
                    u8LoU8 = (u8)(kpku16Dat[u16i] & 0x00FFu);
                    u8HiU8 = (u8)((kpku16Dat[u16i] & 0xFF00u) >> 8u);

                    HdlIicWrU8(u8HiU8);
                    eIicAckTyp = HdlIicWaitAck();

                    if(eIicAckTyp == IicAckTypAck)
                    {
                        HdlIicWrU8(u8LoU8);
                        eIicAckTyp = HdlIicWaitAck();
                    }
                }while((eIicAckTyp == IicAckTypAck) && (++u16i < ku16U16Amt));
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
 * @fn dtc RdIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, u8* const kpu8Dat)
 * @brief IIC主设备读从机连续u8数据。
 * @details 先读数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U8Amt 读取u8数。
 * @param[out] kpu8Dat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, u8* const kpu8Dat)
{
    u16 u16i = 0u;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrU8(ku8RegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            HdlIicStrt();
            HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypRd);

            if(HdlIicWaitAck() == IicAckTypAck)
            {
                HdlIicRdU8(&kpu8Dat[u16i++]);

                while(u16i < ku16U8Amt)
                {
                    HdlIicAck();
                    HdlIicRdU8(&kpu8Dat[u16i++]);
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
 * @fn dtc RdIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
 *                    const EEndn keEndn, u16* const kpu16Dat)
 * @brief IIC主设备读从机连续u16数据。
 * @details 先读数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U16Amt 读取u16数。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] kpu16Dat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
               const EEndn keEndn, u16* const kpu16Dat)
{
    u8 u8LoU8 = 0u;
    u8 u8HiU8 = 0u;
    u16 u16i = 0u;
    dtc dtcRtn = DTC_OK;

    HdlIicStrt();
    HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypWr);

    if(HdlIicWaitAck() == IicAckTypAck)
    {
        HdlIicWrU8(ku8RegAdr);

        if(HdlIicWaitAck() == IicAckTypAck)
        {
            HdlIicStrt();
            HdlIicWrU8((ku8SlvAdr << 1u) | IicRwTypRd);

            if(HdlIicWaitAck() == IicAckTypAck)
            {
                if(keEndn == EndnLe)
                {
                    HdlIicRdU8(&u8LoU8);
                    HdlIicAck();
                    HdlIicRdU8(&u8HiU8);
                    kpu16Dat[u16i++] = (u16)u8LoU8 | ((u16)u8HiU8 << 8u);

                    while(u16i < ku16U16Amt)
                    {
                        HdlIicAck();
                        HdlIicRdU8(&u8LoU8);
                        HdlIicAck();
                        HdlIicRdU8(&u8HiU8);
                        kpu16Dat[u16i++] = (u16)u8LoU8 | ((u16)u8HiU8 << 8u);
                    }
                }
                else if(keEndn == EndnBe)
                {
                    HdlIicRdU8(&u8HiU8);
                    HdlIicAck();
                    HdlIicRdU8(&u8LoU8);
                    kpu16Dat[u16i++] = (u16)u8LoU8 | ((u16)u8HiU8 << 8u);

                    while(u16i < ku16U16Amt)
                    {
                        HdlIicAck();
                        HdlIicRdU8(&u8HiU8);
                        HdlIicAck();
                        HdlIicRdU8(&u8LoU8);
                        kpu16Dat[u16i++] = (u16)u8LoU8 | ((u16)u8HiU8 << 8u);
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
 * @fn dtc WrIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
 *                       const u8* const kpku8Dat)
 * @brief IIC主设备写连续u8数据到从机，带故障重试。
 * @details 先写数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U8Amt 写入u8数。
 * @param[in] kpku8Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, const u8* const kpku8Dat)
{
    u8 u8WrCnt = 0u;
    dtc dtcWrIicSerU8 = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerU8 = WrIicSerU8(ku8SlvAdr, ku8RegAdr, ku16U8Amt, kpku8Dat);
        u8WrCnt++;
    }while((dtcWrIicSerU8 != DTC_OK) && (u8WrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerU8 != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
 *                       const EEndn keEndn, const u16* const kpku16Dat)
 * @brief IIC主设备写连续u16数据到从机，带故障重试。
 * @details 先读数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U16Amt 写入u16数。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] kpku16Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                  const EEndn keEndn, const u16* const kpku16Dat)
{
    u8 u8WrCnt = 0u;
    dtc dtcWrIicSerU16 = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcWrIicSerU16 = WrIicSerU16(ku8SlvAdr, ku8RegAdr, ku16U16Amt, keEndn, kpku16Dat);
        u8WrCnt++;
    }while((dtcWrIicSerU16 != DTC_OK) && (u8WrCnt < SFT_IIC_RETRY));

    if(dtcWrIicSerU16 != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, u8* const kpu8Dat)
 * @brief IIC主设备读从机连续u8数据，带故障重试。
 * @details 先读数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U8Amt 读取u8数。
 * @param[out] kpu8Dat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, u8* const kpu8Dat)
{
    u8 u8WrCnt = 0u;
    dtc dtcRdIicSerU8 = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcRdIicSerU8 = RdIicSerU8(ku8SlvAdr, ku8RegAdr, ku16U8Amt, kpu8Dat);
        u8WrCnt++;
    }while((dtcRdIicSerU8 != DTC_OK) && (u8WrCnt < SFT_IIC_RETRY));

    if(dtcRdIicSerU8 != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc RdIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
 *                       const EEndn keEndn, u16* const kpu16Dat)
 * @brief IIC主设备读从机连续u16数据，带故障重试。
 * @details 先读数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U16Amt 读取u16数。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] kpu16Dat 数据存储数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc RdIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                  const EEndn keEndn, u16* const kpu16Dat)
{
    u8 u8WrCnt = 0u;
    dtc dtcRdIicSerU16 = DTC_OK;
    dtc dtcRtn = DTC_OK;

    do
    {
        dtcRdIicSerU16 = RdIicSerU16(ku8SlvAdr, ku8RegAdr, ku16U16Amt, keEndn, kpu16Dat);
        u8WrCnt++;
    }while((dtcRdIicSerU16 != DTC_OK) && (u8WrCnt < SFT_IIC_RETRY));

    if(dtcRdIicSerU16 != DTC_OK)
    {
        dtcRtn = DTC_ERR;
    }

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerU8Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
 *                       const u8* const kpku8Dat)
 * @brief IIC主设备写连续u8数据到从机，带回读重试。
 * @details 可控制u8数。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U8Amt 写入u8数。
 * @param[in] kpku8Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU8Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt, const u8* const kpku8Dat)
{
    u8 u8WrCnt = 0u;
    bl bEq = FALSE;
    dtc dtcWrIicSerU8 = DTC_OK;
    dtc dtcRdIicSerU8 = DTC_OK;
    dtc dtcRtn = DTC_OK;
    u8* pu8RclDat = (u8*)malloc(ku16U8Amt * sizeof(u8));

    do
    {
        dtcWrIicSerU8 = WrIicSerU8Chk(ku8SlvAdr, ku8RegAdr, ku16U8Amt, kpku8Dat);
        dtcRdIicSerU8 = RdIicSerU8Chk(ku8SlvAdr, ku8RegAdr, ku16U8Amt, pu8RclDat);

        if((dtcWrIicSerU8 == DTC_OK) && (dtcRdIicSerU8 == DTC_OK))
        {
            if(CmpU8(kpku8Dat, pu8RclDat, ku16U8Amt))
            {
                bEq = TRUE;
            }
        }

        u8WrCnt++;
    }while((u8WrCnt < SFT_IIC_RETRY) && (bEq == FALSE));

    if(bEq == FALSE)
    {
        dtcRtn = DTC_ERR;
    }

    free(pu8RclDat);

    return dtcRtn;
}

/**
 * @fn dtc WrIicSerU16Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
 *                       const EEndn keEndn, const u16* const kpku16Dat)
 * @brief IIC主设备写连续u16数据到从机，带回读重试。
 * @details 先发数组小下标。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku16U16Amt 写入u16数。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] kpku16Dat 写入数据数组指针。
 * @return 故障检测码。
 * @note 7bit地址。
 */
dtc WrIicSerU16Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                  const EEndn keEndn, const u16* const kpku16Dat)
{
    u8 u8WrCnt = 0u;
    bl bEq = FALSE;
    dtc dtcWrIicSerU16 = DTC_OK;
    dtc dtcRdIicSerU16 = DTC_OK;
    dtc dtcRtn = DTC_OK;
    u16* pu16RclDat = (u16*)malloc(ku16U16Amt * sizeof(u16));

    do
    {
        dtcWrIicSerU16 = WrIicSerU16Chk(ku8SlvAdr, ku8RegAdr, ku16U16Amt, keEndn, kpku16Dat);
        dtcRdIicSerU16 = RdIicSerU16Chk(ku8SlvAdr, ku8RegAdr, ku16U16Amt, keEndn, pu16RclDat);

        if((dtcWrIicSerU16 == DTC_OK) && (dtcRdIicSerU16 == DTC_OK))
        {
            if(CmpU16(kpku16Dat, pu16RclDat, ku16U16Amt))
            {
                bEq = TRUE;
            }
        }

        u8WrCnt++;
    }while((u8WrCnt < SFT_IIC_RETRY) && (bEq == FALSE));

    if(bEq == FALSE)
    {
        dtcRtn = DTC_ERR;
    }

    free(pu16RclDat);

    return dtcRtn;
}

/**
 * @fn dtc WrIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Dat)
 * @brief IIC主设备写u8数据到从机。
 * @details 写入u8数据。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku8Dat 写数据。
 * @return 故障检测码。
 */
dtc WrIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Dat)
{
    u8 u8Dat = ku8Dat;

    return WrIicSerU8Rcl(ku8SlvAdr, ku8RegAdr, 1u, &u8Dat);
}

/**
 * @fn dtc WrIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, const u16 ku16Dat)
 * @brief IIC主设备写u16数据到从机。
 * @details 区分字节序。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] ku16Dat 写数据。
 * @return 故障检测码。
 */
dtc WrIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, const u16 ku16Dat)
{
    u16 u16Dat = ku16Dat;

    return WrIicSerU16Rcl(ku8SlvAdr, ku8RegAdr, 1u, keEndn, &u16Dat);
}

/**
 * @fn dtc RdIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, u8* const kpu8Dat)
 * @brief IIC主设备读从机u8数据。
 * @details 读取u8数据。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[out] kpu8Dat 读取数据。
 * @return 故障检测码。
 */
dtc RdIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, u8* const kpu8Dat)
{
    return RdIicSerU8Chk(ku8SlvAdr, ku8RegAdr, 1u, kpu8Dat);
}

/**
 * @fn dtc RdIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, u16* const kpu16Dat)
 * @brief IIC主设备读从机u16数据。
 * @details 区分字节序。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[out] kpu16Dat 读取数据。
 * @return 故障检测码。
 */
dtc RdIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, u16* const kpu16Dat)
{
    return RdIicSerU16Chk(ku8SlvAdr, ku8RegAdr, 1u, keEndn, kpu16Dat);
}

/**
 * @fn dtc ModU8Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Map, const u8 ku8Md)
 * @brief 修改u8位。
 * @details 置位或复位寄存器位。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku8Map 寄存器映射位。
 * @param[in] ku8Md 模式。
 * @arg RESET 复位映射位。
 * @arg SET 置位映射位。
 * @return 故障检测码。
 */
dtc ModU8Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Map, const u8 ku8Md)
{
    u8 u8Tmp = 0u;
    dtc dtcRtn = DTC_OK;

    if(!RdIicU8(ku8SlvAdr, ku8RegAdr, &u8Tmp))
    {
        u8Tmp = ku8Md ? SetMapBit(u8Tmp, ku8Map) : RstMapBit(u8Tmp, ku8Map);

        if(WrIicU8(ku8SlvAdr, ku8RegAdr, u8Tmp))
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
 * @fn dtc ModU16Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Md,
 *                  const EEndn keEndn, const u16 ku16Map)
 * @brief 修改u16位。
 * @details 置位或复位寄存器位，区分字节序。
 * @param[in] ku8SlvAdr 从机地址。
 * @param[in] ku8RegAdr 寄存器地址。
 * @param[in] ku8Md 模式。
 * @arg RESET 复位映射位。
 * @arg SET 置位映射位。
 * @param[in] keEndn 字节序。
 * @arg EndnLe 小端。
 * @arg EndnBe 大端。
 * @param[in] ku16Map 寄存器映射位。
 * @return 故障检测码。
 */
dtc ModU16Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Md, const EEndn keEndn,
             const u16 ku16Map)
{
    u16 u16Tmp = 0u;
    dtc dtcRtn = DTC_OK;

    if(!RdIicU16(ku8SlvAdr, ku8RegAdr, keEndn, &u16Tmp))
    {
        u16Tmp = ku8Md ? SetMapBit(u16Tmp, ku16Map) : RstMapBit(u16Tmp, ku16Map);

        if(WrIicU16(ku8SlvAdr, ku8RegAdr, keEndn, u16Tmp))
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
