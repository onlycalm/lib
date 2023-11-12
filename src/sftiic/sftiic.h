/**
 * @file sftiic.h
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
extern dtc WrIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt,
                      const byte* const cpabyDat);
extern dtc WrIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                      const EEndn ceEndn, const word* const cpawDat);
extern dtc RdIicSerBy(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt,
                      byte* const cpabyDat);
extern dtc RdIicSerWd(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                      const EEndn ceEndn, word* const cpawDat);
extern dtc WrIicSerByChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt,
                         const byte* const cpabyDat);
extern dtc WrIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                         const EEndn ceEndn, const word* const cpawDat);
extern dtc RdIicSerByChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt,
                         byte* const cpabyDat);
extern dtc RdIicSerWdChk(const byte cbySlvAdr, const byte cbyRegAdr, const word cwWdAmt,
                         const EEndn ceEndn, word* const cpawDat);
extern dtc WrIicSerByRcl(const byte cbySlvAdr, const byte cbyRegAdr, const word cwByAmt,
                         const byte* const cpabyDat);
extern dtc WrIicSerWdRcl(const byte cbySlvAdr, const byte cRegAdr, const word cwWdAmt,
                         const EEndn ceEndn, const word* const cpawDat);
extern dtc WrIicBy(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyDat);
extern dtc WrIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, const word cwDat);
extern dtc RdIicBy(const byte cbySlvAdr, const byte cbyRegAdr, byte* const cpbyDat);
extern dtc RdIicWd(const byte cbySlvAdr, const byte cbyRegAdr, const EEndn ceEndn, word* const cpwDat);
extern dtc ModByBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMap, const byte cbyMd);
extern dtc ModWdBit(const byte cbySlvAdr, const byte cbyRegAdr, const byte cbyMd,
                    const EEndn ceEndn, const word cwMap);

#endif //SFT_IIC_H
