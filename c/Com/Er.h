/**
 * @file er.h
 * @brief er Module
 * @details This file contains type definitions and function declarations for
 *          the GPIO module, used for initializing and controlling GPIO pins.
 * @author Calm
 * @data 2025-12-21
 * @version v1.0.0
 * @copyright (C) 2025 Ordinary People. This project is open source under
              the MIT License.
 */

#ifndef __ER_H__
#define __ER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "Typ.h"
#include "Log.h"

#ifndef ER_DOM
#error Please define ER_DOM
#endif

#ifndef ER_SUB_DOM
#error Please define ER_SUB_DOM
#endif

#define ER_LV_BT_FLD      0x07u
#define ER_CD_BT_FLD      0x3Fu
#define ER_CLS_BT_FLD     0x1Fu
#define ER_MOD_BT_FLD     0xFFu
#define ER_SUB_DOM_BT_FLD 0x0Fu
#define ER_DOM_BT_FLD     0x0Fu
#define ER_RSV_BT_FLD     0x03u

#define u32PkgEr(eLv, eCd, eCls, eMod, eSubDom, eDom) \
    ((((u32)(eLv)&ER_LV_BT_FLD) << 0) | \
     (((u32)(eCd)&ER_CD_BT_FLD) << 3) | \
     (((u32)(eCls)&ER_CLS_BT_FLD) << 9) | \
     (((u32)(eMod)&ER_MOD_BT_FLD) << 14) | \
     (((u32)(eSubDom)&ER_SUB_DOM_BT_FLD) << 22) | \
     (((u32)(eDom)&ER_DOM_BT_FLD) << 26))

#define u32GetErLv(erEr)     ((u32)(((erEr) >> 0) & ER_LV_BT_FLD))
#define u32GetErCd(erEr)     ((u32)(((erEr) >> 3) & ER_CD_BT_FLD))
#define u32GetErCls(erEr)    ((u32)(((erEr) >> 9) & ER_CLS_BT_FLD))
#define u32GetErMod(erEr)    ((u32)(((erEr) >> 14) & ER_MOD_BT_FLD))
#define u32GetErSubDom(erEr) ((u32)(((erEr) >> 22) & ER_SUB_DOM_BT_FLD))
#define u32GetErDom(erEr)    ((u32)(((erEr) >> 26) & ER_DOM_BT_FLD))

#define u32PkgClsEr(eLv, eCd, eCls) u32PkgEr((eLv), (eCd), (eCls), (ER_MOD), \
                                             (ER_SUB_DOM), (ER_DOM))
#define ER_LV_TO_LOG_LV(erEr) \
    ((u32GetErLv(erEr) >= ER_LV_CRIT) ? LOG_CRITICAL : \
     (u32GetErLv(erEr) >= ER_LV_ER) ? LOG_ERROR : \
     (u32GetErLv(erEr) >= ER_LV_WRN) ? LOG_WARNING : \
     (u32GetErLv(erEr) >= ER_LV_SUC) ? LOG_SUCCESS : \
     LOG_ERROR)
#define ER_PRN_LOG(erEr) \
    erEr; \
    Log(ER_LV_TO_LOG_LV((erEr)), "er: 0x%08lX", (u32)(erEr))

// Error code.
#define ER_SUC 0u
#define ER_SUC_ ER_PRN_LOG(ER_SUC)

// Software class.
#define ER_SW_UNKN \
    u32PkgClsEr((ER_LV_ER), (ER_CD_SW_UNKN), (ER_CLS_SW))
#define ER_SW_UNKN_ ER_PRN_LOG(ER_SW_UNKN)

#define ER_SW_NUL_PTR \
    u32PkgClsEr((ER_LV_ER), (ER_CD_SW_NUL_PTR), (ER_CLS_SW))
#define ER_SW_NUL_PTR_ ER_PRN_LOG(ER_SW_NUL_PTR)

#define ER_SW_INV_PARAM \
    u32PkgClsEr((ER_LV_ER), (ER_CD_SW_INV_PARAM), (ER_CLS_SW))
#define ER_SW_INV_PARAM_ ER_PRN_LOG(ER_SW_INV_PARAM)

// Communication class.
#define ER_COMM_TX_TMOT \
    u32PkgClsEr((ER_LV_ER), (ER_CD_COMM_TX_TMOT), (ER_CLS_COMM))
#define ER_COMM_TX_TMOT_ ER_PRN_LOG(ER_COMM_TX_TMOT)

#define ER_COMM_BSY \
    u32PkgClsEr((ER_LV_ER), (ER_CD_COMM_TX_BSY), (ER_CLS_COMM))
#define ER_COMM_BSY_ ER_PRN_LOG(ER_COMM_BSY)

// Data class.
#define ER_DAT_OVF \
    u32PkgClsEr((ER_LV_ER), (ER_CD_DAT_OVF), (ER_CLS_DAT))
#define ER_DAT_OVF_ ER_PRN_LOG(ER_DAT_OVF)

// File class.
#define ER_FILE_INV_PTH \
    u32PkgClsEr((ER_LV_ER), (ER_CD_FILE_INV_PTH), (ER_CLS_FILE))
#define ER_FILE_INV_PTH_ ER_PRN_LOG(ER_FILE_INV_PTH)

#define ER_FILE_CRT_DIR_FAIL \
    u32PkgClsEr((ER_LV_ER), (ER_CD_FILE_CRT_DIR_FAIL), (ER_CLS_FILE))
#define ER_FILE_CRT_DIR_FAIL_ ER_PRN_LOG(ER_FILE_CRT_DIR_FAIL)

typedef union
{
    struct
    {
        u32 btLv     : 3; // [2:0] Error level.
        u32 btCd     : 6; // [8:3] Error code.
        u32 btCls    : 5; // [13:9] Class.
        u32 btMod    : 8; // [21:14] Module ID.
        u32 btSubDom : 4; // [25:22] Sub-Domain.
        u32 btDom    : 4; // [29:26] Domain.
        u32 Rsv      : 2; // [31:30] Reserved.
    } bt;

    u32 al;
} UEr;

typedef enum
{
    ER_LV_SUC, // The other bits should be zero, indicating no fault.
    ER_LV_WRN,
    ER_LV_ER,
    ER_LV_CRIT,

    ER_LV_MAX = ER_LV_BT_FLD + 1u,
} EErLv;

typedef enum
{
    ER_CD_SW_UNKN,         // Unknown.
    ER_CD_SW_NUL_PTR,      // Null Pointer.
    ER_CD_SW_INV_PARAM,    // Invalid Parameter.

    ER_CD_SW_MAX = ER_CD_BT_FLD + 1u,
} EErCdSw;

typedef enum
{
    ER_CD_COMM_ACK_TMOT,   // Timeout Error.
    ER_CD_COMM_TX_BUF_FUL, // Transmit Buffer Full.
    ER_CD_COMM_RX_BUF_FUL, // Receive Buffer Full.
    ER_CD_COMM_TX_TMOT,    // Transmit timeout.
    ER_CD_COMM_RX_TMOT,    // Receive timeout.
    ER_CD_COMM_TX_BSY,     // Transmit busy.

    ER_CD_COMM_MAX = ER_CD_BT_FLD + 1u,
} EErCdComm;

typedef enum
{
    ER_CD_PERI_INIT, // Peripheral Initialization Error.
    ER_CD_PERI_CFG,  // Peripheral Configuration Error.

    ER_CD_PERI_MAX = ER_CD_BT_FLD + 1u,
} EErCdPeri;

typedef enum
{
    ER_CD_SNSR_DISCONN, // Sensor Disconnected.
    ER_CD_SNSR_CFG,     // Sensor Configuration Error.

    ER_CD_SNSR_MAX = ER_CD_BT_FLD + 1u,
} EErCdSnsr;

typedef enum
{
    ER_CD_ACT_STALL, // Actuator Stalled.

    ER_CD_ACT_MAX = ER_CD_BT_FLD + 1u,
} EErCdAct;

typedef enum
{
    ER_CD_MEM_ECC,      // Memory ECC Error.
    ER_CD_MEM_WRT_FAIL, // Memory Write Failure.
    ER_CD_MEM_RD_FAIL,  // Memory Read Failure.

    ER_CD_MEM_MAX = ER_CD_BT_FLD + 1u,
} EErCdMem;

typedef enum
{
    ER_CD_PROC_BRG_UP, // Bring up Error.
    ER_CD_PROC_BRG_DN, // Bring down Error.

    ER_CD_PROC_MAX = ER_CD_BT_FLD + 1u,
} EErCdProc;

typedef enum
{
    ER_CD_VRFY_AUTH_FAIL, // Authentication Failure.
    ER_CD_VRFY_ENC_FAIL,  // Encryption Failure.
    ER_CD_VRFY_CRC_FAIL,  // CRC Check Failure.

    ER_CD_VRFY_MAX = ER_CD_BT_FLD + 1u,
} EErCdVrfy;

typedef enum
{
    ER_CD_OS_TSK_LD_HIGH, // Task Load Too High.
    ER_CD_OS_MEM_LD_HIGH, // Memory Load Too High.
    ER_CD_OS_STK_HP_LD_HIGH, // Stack/Heap Load Too High.

    ER_CD_OS_MAX = ER_CD_BT_FLD + 1u,
} EErCdOs;

typedef enum
{
    ER_CD_ENV_TEMP_HIGH, // Temperature Too Hight.
    ER_CD_ENV_TEMP_LOW,  // Temperature Too Low.

    ER_CD_ENV_MAX = ER_CD_BT_FLD + 1u,
} EErCdEnv;

typedef enum
{
    ER_CD_DAT_OVF, // Data Overflow.

    ER_CD_DAT_MAX = ER_CD_BT_FLD + 1u,
} EErCdDat;

typedef enum
{
    ER_CD_FILE_INV_PTH,      // Invalid Path.
    ER_CD_FILE_CRT_DIR_FAIL, // Create Directory Failure.

    ER_CD_FILE_MAX = ER_CD_BT_FLD + 1u,
} EErCdFile;

typedef enum
{
    ER_CLS_SW,   // Software Class.
    ER_CLS_COMM, // Communication Class.
    ER_CLS_PERI, // Peripheral Class.
    ER_CLS_SNSR, // Sensor Class.
    ER_CLS_ACT,  // Actuator Class.
    ER_CLS_MEM,  // Memory Class.
    ER_CLS_PROC, // Processor Class.
    ER_CLS_VRFY, // Verify Class.
    ER_CLS_OS,   // Operating System Class.
    ER_CLS_ENV,  // Environment Clss.
    ER_CLS_DAT,  // Data Class.
    ER_CLS_FILE, // File Class.

    ER_CLS_MAX = ER_CLS_BT_FLD + 1u,
} EErCls;

typedef enum
{
    ER_MOD_GPIO, // Gpio Module.
    ER_MOD_CLK,  // Clk Module.
    ER_MOD_COM,  // Common Module.
    ER_MOD_INTR, // Intr Module.
    ER_MOD_URT,  // Uart Module.
    ER_MOD_LOG,  // Log Module.

    ER_MOD_MAX = ER_MOD_BT_FLD + 1u,
} EErMod;

typedef enum
{
    ER_SUB_DOM_ORDMCU,
    ER_SUB_DOM_ORDRTOS,
    ER_SUB_DOM_ORDBOT,

    ER_SUB_DOM_MAX = ER_SUB_DOM_BT_FLD + 1u,
} EErSubDom;

typedef enum
{
    ER_DOM_ORD_BOT,
    ER_DOM_BAL_CAR,

    ER_DOM_MAX = ER_DOM_BT_FLD + 1u,
} EErDom;

#ifdef __cplusplus
}
#endif

#endif // __ER_H__
