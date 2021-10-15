/**
 * @file sftiic.h
 * @brief 软件IIC模块。
 * @details None
 * @author Calm
 * @date 2021-06-15
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef SFT_IIC_H
#define SFT_IIC_H

#include "hdr.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
#ifndef SFT_IIC_WAIT_ACK_TM
#define SFT_IIC_WAIT_ACK_TM             7u                      //!<约2us，等待ACK时间。
#endif //SFT_IIC_WAIT_ACK_TM

#ifndef SFT_IIC_DLY
#define SFT_IIC_DLY                     2u                      //!<延迟2us，控制波特率约120kHz。
#endif //SFT_IIC_DLY

#ifndef SFT_IIC_DLY_HLF
#define SFT_IIC_DLY_HLF                 (SFT_IIC_DLY >> 1u)     //!<延迟时间一半。
#endif //SFT_IIC_DLY_HLF

#ifndef SFT_IIC_RETRY
#define SFT_IIC_RETRY                   3u                      //!<重试次数。
#endif //SFT_IIC_RETRY

/*****************************************************************************
 *变量声明                                                                   *
 *****************************************************************************/

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//中断函数

//=============================================================================
//内敛函数

//=============================================================================
//普通函数
extern DWord WrIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pbyDat);
extern DWord RdIicDat(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pbyDat);
extern DWord WrIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat);
extern DWord RdIicDatChk(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat);
extern DWord WrIicDatRcl(Byte bySlvAddr, Byte RegAddr, Byte byByteAmt, Byte* pabyDat);

/*****************************************************************************
 *内敛函数定义                                                               *
 *****************************************************************************/
/**
 * @fn static __forceinline void SetSdaIn(void)
 * @brief 设置SDA为输入IO口。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void SetSdaIn(void)
{
    bM4_PORT_PCRA11_NOD = Pin_OType_Cmos;
    bM4_PORT_PCRA11_POUTE = 0u;
}

/**
 * @fn static __forceinline void SetSdaOd(void)
 * @brief 设置SDA为开漏输出IO口。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void SetSdaOd(void)
{
    bM4_PORT_PCRA11_NOD = Pin_OType_Od;
    bM4_PORT_PCRA11_POUTE = 1u;
}

/**
 * @fn static __forceinline void SetSdaPol(Bool bPol)
 * @brief 设置SDA极性。
 * @details 无
 * @param bPol
 * @arg HIGH 高。
 * @arg LOW 低。
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void SetSdaPol(Bool bPol)
{
    SetPinPol(PT_IIC_SDA, PIN_IIC_SDA, bPol);
}

/**
 * @fn static __forceinline void SetSclPol(Bool bPol)
 * @brief 设置SCL极性。
 * @details 无
 * @param bPol
 * @arg HIGH 高。
 * @arg LOW 低。
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void SetSclPol(Bool bPol)
{
    SetPinPol(PT_IIC_SCL, PIN_IIC_SCL, bPol);
}

/**
 * @fn static __forceinline void GetSdaPol(void)
 * @brief 获取SDA极性。
 * @details 无
 * @param void
 * @return 极性。
 * @retval HIGH 高。
 * @retval LOW 低。
 * @note 无
 * @attention 无
 */
static __forceinline Bool GetSdaPol(void)
{
    return GetPinPol(PT_IIC_SDA, PIN_IIC_SDA);
}

/**
 * @fn static __forceinline void HdlIicStrt(void)
 * @brief 处理IIC Start动作。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void HdlIicStrt(void)
{
    SetSdaPol(HIGH);
    SetSclPol(HIGH);
    DelayUs(SFT_IIC_DLY);
    SetSdaPol(LOW);
    DelayUs(SFT_IIC_DLY_HLF);
    SetSclPol(LOW);
}

/**
 * @fn static __forceinline void HdlIicStp(void)
 * @brief 处理IIC Stop动作。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void HdlIicStp(void)
{
    SetSdaPol(LOW);
    SetSclPol(LOW);
    DelayUs(SFT_IIC_DLY_HLF);
    SetSclPol(HIGH);
    DelayUs(SFT_IIC_DLY_HLF);
    SetSdaPol(HIGH);
    DelayUs(SFT_IIC_DLY);
}

/**
 * @fn static __forceinline void HdlIicAck(void)
 * @brief 处理IIC Ack动作。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void HdlIicAck(void)
{
    SetSdaPol(LOW);
    SetSclPol(LOW);
    DelayUs(SFT_IIC_DLY);
    SetSclPol(HIGH);
    DelayUs(SFT_IIC_DLY);
    SetSclPol(LOW);
    DelayUs(SFT_IIC_DLY);
}

/**
 * @fn static __forceinline void HdlIicNAck(void)
 * @brief 处理IIC NAck动作。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void HdlIicNAck(void)
{
    SetSclPol(LOW);
    SetSdaPol(HIGH);
    DelayUs(SFT_IIC_DLY);
    SetSclPol(HIGH);
    DelayUs(SFT_IIC_DLY);
    SetSclPol(LOW);
}

#endif //SFT_IIC_H
