/**
 * @file SftIic.h
 * @brief 软件IIC模块。
 * @details IIC主机。
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
//=============================================================================
//参数配置
#ifndef SFT_IIC_RETRY
#define SFT_IIC_RETRY              3u       //!<重试次数。
#endif //SFT_IIC_RETRY

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
extern dtc WrIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
                      const u8* const kpku8Dat);
extern dtc WrIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                      const EEndn keEndn, const u16* const kpku16Dat);
extern dtc RdIicSerU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
                      u8* const kpu8Dat);
extern dtc RdIicSerU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                      const EEndn keEndn, u16* const kpu16Dat);
extern dtc WrIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
                         const u8* const kpku8Dat);
extern dtc WrIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                         const EEndn keEndn, const u16* const kpku16Dat);
extern dtc RdIicSerU8Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
                         u8* const kpu8Dat);
extern dtc RdIicSerU16Chk(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                         const EEndn keEndn, u16* const kpu16Dat);
extern dtc WrIicSerU8Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U8Amt,
                         const u8* const kpku8Dat);
extern dtc WrIicSerU16Rcl(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u16 ku16U16Amt,
                         const EEndn keEndn, const u16* const kpku16Dat);
extern dtc WrIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Dat);
extern dtc WrIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, const u16 ku16Dat);
extern dtc RdIicU8(const u8 ku8SlvAdr, const u8 ku8RegAdr, u8* const kpu8Dat);
extern dtc RdIicU16(const u8 ku8SlvAdr, const u8 ku8RegAdr, const EEndn keEndn, u16* const kpu16Dat);
extern dtc ModU8Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Map, const u8 ku8Md);
extern dtc ModU16Bit(const u8 ku8SlvAdr, const u8 ku8RegAdr, const u8 ku8Md,
                     const EEndn keEndn, const u16 ku16Map);

#endif //SFT_IIC_H
