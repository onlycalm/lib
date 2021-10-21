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
#ifndef BYTE
#define BYTE byte
#endif //BYTE

#ifndef WORD
#define WORD word
#endif //WORD

#ifndef DWORD
#define DWORD dword
#endif //DWORD

#ifndef BOOL
#define BOOL byte
#endif //BOOL

#ifndef DTC
#define DTC dtc
#endif //DTC

#endif //TYPE_H
