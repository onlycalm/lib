/**
 * @file type.h
 * @brief 类型定义。
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef TYPE_H
#define TYPE_H

#include "hdr.h"

/*****************************************************************************
 *类型定义                                                                   *
 *****************************************************************************/
typedef uint8_t bool;
typedef uint8_t boolean;
typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint32_t dtc;
typedef char* pchar;

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//类型定义
#ifndef BOOL
#define BOOL uint8_t
#endif //BOOL

#ifndef BOOLEAN
#define BOOLEAN uint8_t
#endif //BOOLEAN

#ifndef BYTE
#define BYTE uint8_t
#endif //BYTE

#ifndef WORD
#define WORD uint16_t
#endif //WORD

#ifndef DWORD
#define DWORD uint32_t
#endif //DWORD

#ifndef DTC
#define DTC uint32_t
#endif //DTC

#ifndef PCHAR
#define PCHAR char*
#endif //PCHAR

//=============================================================================
//类型大小
#ifndef BY_SZ
#define BY_SZ             1u            //!<byte占1字节。
#endif //BY_SZ

#ifndef WD_SZ
#define WD_SZ             2u            //!<word占2字节。
#endif //WD_SZ

#ifndef DWD_SZ
#define DWD_SZ            4u            //!<dword占4字节。
#endif //DWD_SZ

#ifndef DTC_SZ
#define DTC_SZ            4u            //!<dtc占4字节。
#endif //DTC_SZ

/*****************************************************************************
 *枚举定义                                                                   *
 *****************************************************************************/
/**
 * @enum DatTyp
 * @brief 数据类型。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef enum DatTyp
{
    DatTypBy,
    DatTypWd,
    DatTypDwd
}EDatTyp;

#endif //TYPE_H
