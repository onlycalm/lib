/**
 * @file com.h
 * @brief 通用文件。
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
//环境依赖宏定义。
#ifndef WEAK
#define WEAK                        __WEAK
#endif //WEAK

#ifndef INLINE
#define INLINE                      __INLINE
#endif //INLINE

#ifndef STC_INLINE
#define STC_INLINE                  __STATIC_INLINE
#endif //STC_INLINE

#ifndef STC_FRC_INLINE
#define STC_FRC_INLINE              __STATIC_FORCEINLINE
#endif //STC_FRC_INLINE

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

#ifndef SET
#define SET                         1u
#endif //SET

#ifndef RST
#define RST                         0u
#endif //RST

#ifndef ENDN_LE
#define ENDN_LE                     0u              //!<小端模式。
#endif //ENDN_LE

#ifndef ENDN_BE
#define ENDN_BE                     1u              //!<大端模式。
#endif //ENDN_BE

//=============================================================================
//宏函数定义
#define ToStr(x)                 Str(x)                      //!<宏值转字符串。
#define Str(x)                   #x                          //!<名转字符串。
#define DoNthg(void)                                         //!<空语句。
#define Sl(Val, N)               ((Val) << (N))              //!<左移位。
#define Sr(Val, N)               ((Val) >> (N))              //!<右移位。
#define SetMapBit(Val, Map)      ((Val) | (Map))             //!<置位映射位。
#define RstMapBit(Val, Map)      ((Val) & ~(Map))            //!<复位映射位。
#define SetFld(Val, Ofst, Fld)   ((Val) << (Ofst) & (Fld))   //!<设置字段。
#define RndBy(fVal)              (byte)((fVal) + 0.5f)       //!<byte四舍五入。
#define RndWd(fVal)              (word)((fVal) + 0.5f)       //!<word四舍五入。
#define RndDwd(fVal)             (dword)((fVal) + 0.5f)      //!<dword四舍五入。
#define ToBy(Val)                ((byte)(Val))               //!<转byte。
#define ToWd(Val)                ((word)(Val))               //!<转word。
#define ToDwd(Val)               ((dword)(Val))              //!<转dword。

/*****************************************************************************
 *枚举定义                                                                   *
 *****************************************************************************/
/**
 * @enum Endn
 * @brief 字节序。
 * @details 分为大端和小端两种。
 */
typedef enum Endn
{
    EndnLe, //!<little-endian小端。
    EndnBe  //!<big-endian大端。
}EEndn;

/*****************************************************************************
 *联合定义                                                                   *
 *****************************************************************************/
/**
 * @enum ByFld
 * @brief 带位域byte。
 * @details 可对Bit直接操作。
 */
typedef union ByFld
{
    byte byDat; //byte数据。

    struct
    {
        byte byB0 : 1u; //!<Bit0.
        byte byB1 : 1u; //!<Bit1.
        byte byB2 : 1u; //!<Bit2.
        byte byB3 : 1u; //!<Bit3.
        byte byB4 : 1u; //!<Bit4.
        byte byB5 : 1u; //!<Bit5.
        byte byB6 : 1u; //!<Bit6.
        byte byB7 : 1u; //!<Bit7.
    };
}UByFld;

/**
 * @enum WdFld
 * @brief 带位域word。
 * @details 可对Bit直接操作。
 */
typedef union WdFld
{
    word wDat; //word数据。

    struct
    {
        byte byB0 : 1u;  //!<Bit0.
        byte byB1 : 1u;  //!<Bit1.
        byte byB2 : 1u;  //!<Bit2.
        byte byB3 : 1u;  //!<Bit3.
        byte byB4 : 1u;  //!<Bit4.
        byte byB5 : 1u;  //!<Bit5.
        byte byB6 : 1u;  //!<Bit6.
        byte byB7 : 1u;  //!<Bit7.
        byte byB8 : 1u;  //!<Bit8.
        byte byB9 : 1u;  //!<Bit9.
        byte byB10 : 1u; //!<Bit10.
        byte byB11 : 1u; //!<Bit11.
        byte byB12 : 1u; //!<Bit12.
        byte byB13 : 1u; //!<Bit13.
        byte byB14 : 1u; //!<Bit14.
        byte byB15 : 1u; //!<Bit15.
    };
}UWdFld;

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//普通函数
extern char* GetFileNm(const char* const cpPath);
extern bool CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2, const word cwAmt);
extern bool CmpWd(const word* const cpwDat1, const word* const cpwDat2, const word cwAmt);
extern bool CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2, const word cwAmt);

#endif //COM_H
