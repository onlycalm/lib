/**
 * @file com.h
 * @brief Common module.
 * @details None
 * @author Calm
 * @date 2021-10-14
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef COM_H
#define COM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "typ.h"

/******************************************************************************
 * Macro definition                                                           *
 ******************************************************************************/
// Environmental macro.
#ifndef WEAK
#define WEAK __WEAK
#endif // WEAK

#ifndef INLINE
#define INLINE __INLINE
#endif // INLINE

#ifndef STC_INLINE
#define STC_INLINE __STATIC_INLINE
#endif // STC_INLINE

#ifndef STC_FRC_INLINE
#define STC_FRC_INLINE __STATIC_FORCEINLINE
#endif // STC_FRC_INLINE

// Common macro.
#ifndef FALSE
#define FALSE 0u
#endif // FALSE

#ifndef TRUE
#define TRUE 1u
#endif // TRUE

#ifndef NULL
#define NULL ((void*)0u)
#endif // NULL

#ifndef NO
#define NO 0u
#endif // NO

#ifndef YES
#define YES 1u
#endif // YES

#ifndef DISABLE
#define DISABLE 0u
#endif // DISABLE

#ifndef ENABLE
#define ENABLE 1u
#endif // ENABLE

#ifndef LOW
#define LOW 0u
#endif // LOW

#ifndef HIGH
#define HIGH 1u
#endif // HIGH

#ifndef OFF
#define OFF 0u
#endif // OFF

#ifndef ON
#define ON 1u
#endif // ON

#ifndef SET
#define SET 1u
#endif // SET

#ifndef RST
#define RST 0u
#endif // RST

#ifndef ENDN_LE
#define ENDN_LE 0u //!< little-endian。
#endif // ENDN_LE

#ifndef ENDN_BE
#define ENDN_BE 1u //!< big-endian.
#endif // ENDN_BE

#ifndef IP_V4_SZ
#define IP_V4_SZ 4u
#endif // IP_V4_SZ

#ifndef IP_MIN
#define IP_MIN 0u
#endif // IP_MIN

#ifndef IP_MAX
#define IP_MAX 255u
#endif // IP_MAX

// Macro function definition.
#define pcToStr(x)       pcStr(x)         //!< Convert macro value to string.
#define pcStr(x)         #x               //!< Name to string conversion.
#define DoNthg(void)                      //!< Do nothing.
#define Sl(Val, N)       ((Val) << (N))   //!< Left shift.
#define Sr(Val, N)       ((Val) >> (N))   //!< Right shift.
#define SetBit(Val, Map) ((Val) | (Map))  //!< Set bit.
#define RstBit(Val, Map) ((Val) & ~(Map)) //!< Reset bit.
#define SetBitFld(Val, Ofst, Msk) \
    (((Val) << (Ofst)) & (Msk))             //!< Set bit field.
#define f32Rd(Val) ((float)(Val) + 0.5f)    //!< float rounding.
#define u8RdU8(Val) ((u8)(f32Rd(Val)))      //!< u8 rounding.
#define u8RdU16(Val) ((u16)(f32Rd(Val)))    //!< u16 rounding.
#define u8RdU32(Val) ((u32)(f32Rd(Val)))    //!< u32 rounding.
#define u16MrU16(u8By0, u8By1) ((u16)(SetBitFld((u8By0), 0u, 0x00FFu) | \
                                      SetBitFld((u8By1), 8u, 0xFF00u)))
#define u32MrU32(u8By0, u8By1, u8By2, u8By3) \
    ((u32)(SetBitFld((u8By0), 0u, 0x000000FFu) | \
           SetBitFld((u8By1), 8u, 0x0000FF00u) | \
           SetBitFld((u8By2), 16u, 0x00FF0000u) | \
           SetBitFld((u8By3), 24u, 0xFF000000u)))
#define bInRng(Val, Min, Max) (((Val) >= (Min)) && ((Val) <= (Max)))

/******************************************************************************
 * Enumeration definition                                                     *
 ******************************************************************************/
/**
 * @enum Endn
 * @brief Byte order.
 * @details It is divided into two types: big-endian and little-endian.
 */
typedef enum Endn
{
    EndnLe, //!< little-endian.
    EndnBe, //!< big-endian.

    EndnMax
} EEndn;

/******************************************************************************
 * Union definition                                                           *
 ******************************************************************************/
/**
 * @enum ByFld
 * @brief Byte with bit fields.
 * @details Allows direct operation on bits.
 */
typedef union ByFld
{
    byte byDat; // Byte data.

    struct
    {
        byte byB0 : 1u; //!< Bit0.
        byte byB1 : 1u; //!< Bit1.
        byte byB2 : 1u; //!< Bit2.
        byte byB3 : 1u; //!< Bit3.
        byte byB4 : 1u; //!< Bit4.
        byte byB5 : 1u; //!< Bit5.
        byte byB6 : 1u; //!< Bit6.
        byte byB7 : 1u; //!< Bit7.
    };
} UnByFld;

/**
 * @enum WdFld
 * @brief Word with bit fields.
 * @details Allows direct operation on bits.
 */
typedef union WdFld
{
    word wDat; // Word data.

    struct
    {
        byte byB0  : 1u; //!< Bit0.
        byte byB1  : 1u; //!< Bit1.
        byte byB2  : 1u; //!< Bit2.
        byte byB3  : 1u; //!< Bit3.
        byte byB4  : 1u; //!< Bit4.
        byte byB5  : 1u; //!< Bit5.
        byte byB6  : 1u; //!< Bit6.
        byte byB7  : 1u; //!< Bit7.
        byte byB8  : 1u; //!< Bit8.
        byte byB9  : 1u; //!< Bit9.
        byte byB10 : 1u; //!< Bit10.
        byte byB11 : 1u; //!< Bit11.
        byte byB12 : 1u; //!< Bit12.
        byte byB13 : 1u; //!< Bit13.
        byte byB14 : 1u; //!< Bit14.
        byte byB15 : 1u; //!< Bit15.
    };
} UnWdFld;

/******************************************************************************
 * Function declaration                                                       *
 ******************************************************************************/
//Global function.
extern BOOL CmpBy(const byte* const cpbyDat1, const byte* const cpbyDat2,
                  const word cwAmt);
extern BOOL CmpWd(const word* const cpwDat1, const word* const cpwDat2,
                  const word cwAmt);
extern BOOL CmpDwd(const dword* const cpdwDat1, const dword* const cpdwDat2,
                   const word cwAmt);
extern u16 u16CvtEndn(const u16 ku16Dat);
extern u32 u32CvtEndn(const u32 ku32Dat);
extern err erIpToU32(const char* const kpkcIp, EEndn eEndn, u32* const kpu32Ip);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // COM_H
