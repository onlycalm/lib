/**
 * @file type.h
 * @brief Type define.
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>
#include <string.h>

/******************************************************************************
 * Type define                                                                *
 ******************************************************************************/
// typedef uint8_t bool;
// typedef uint8_t boolean;
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
typedef uint32_t err;
typedef char* pchar;

/******************************************************************************
 * Macro definition                                                           *
 ******************************************************************************/
// Type define.
#ifndef BOOL
#define BOOL uint8_t
#endif // BOOL

#ifndef BOOLEAN
#define BOOLEAN uint8_t
#endif // BOOLEAN

#ifndef BYTE
#define BYTE uint8_t
#endif // BYTE

#ifndef WORD
#define WORD uint16_t
#endif // WORD

#ifndef DWORD
#define DWORD uint32_t
#endif // DWORD

#ifndef ERR
#define ERR uint32_t
#endif // ERR

#ifndef PCHAR
#define PCHAR char*
#endif // PCHAR

// Number of bytes for a type.
#ifndef BY_SZ
#define BY_SZ 1u //!< Number of bytes for the byte type.
#endif // BY_SZ

#ifndef WD_SZ
#define WD_SZ 2u //!< Number of bytes for the word type.
#endif // WD_SZ

#ifndef DWD_SZ
#define DWD_SZ 4u //!< Number of bytes for the dword type.
#endif // DWD_SZ

#ifndef ERR_SZ
#define ERR_SZ 4u //!< Number of bytes for the err type.
#endif // ERR_SZ

/******************************************************************************
 * Enumeration definition                                                     *
 ******************************************************************************/
/**
 * @enum DatTyp
 * @brief Data type.
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
