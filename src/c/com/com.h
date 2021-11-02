/**
 * @file type.h
 * @brief 类型定义。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef COM_H
#define COM_H

#include "hdr.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//通用宏定义。
#ifndef FALSE
#define FALSE                       0u
#endif //FALSE

#ifndef TRUE
#define TRUE                        1u
#endif //TRUE

#ifndef NULL
#define NULL                        ((void*)0u)
#endif //NULL

#ifndef NO
#define NO                          0u
#endif //NO

#ifndef YES
#define YES                         1u
#endif //YES

#ifndef DISABLE
#define DISABLE                     0u
#endif //DISABLE

#ifndef ENABLE
#define ENABLE                      1u
#endif //ENABLE

#ifndef LOW
#define LOW                         0u
#endif //LOW

#ifndef HIGH
#define HIGH                        1u
#endif //HIGH

#ifndef OFF
#define OFF                         0u
#endif //OFF

#ifndef ON
#define ON                          1u
#endif //ON

//=============================================================================
//宏函数定义
#define ToStr(x)                    Str(x)              //!<宏值转字符串。
#define Str(x)                      #x                  //!<名转字符串。
#define DoNthg()                                        //!<什么也不做。

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
extern pchar GetFileNm(pchar pcPath);
extern BOOL CmpByte(byte* pbyDat1, byte* pbyDat2, byte byAmt);
extern dtc Delay10Us(word wUs);
extern dtc Delay1Ms(word wMs);

#endif //COM_H
