/**
 * @file Sta.h
 * @brief 状态机框架。
 * @details 定义状态回调结构体、状态机句柄结构体及状态机 API。
 *          框架本身不含任何具体状态，具体状态与回调表由 StaImpl 提供。
 *          框架为可移植模块，支持多实例：每个状态机持有独立的句柄 stSta。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef STA_H
#define STA_H

#include "Typ.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ===== 结构体定义 ===== */
/**
 * @struct stStaCb
 * @brief 状态回调函数指针结构体。
 * @details 描述单个状态在进入、运行、转换、退出四个阶段的回调。
 *          转换回调入参为当前状态，返回值为下一状态；返回当前状态表示不转换。
 */
typedef struct stStaCb
{
    void (*pfvidEnt)(void);      //!< 进入状态函数指针。
    void (*pfvidRun)(void);      //!< 运行状态函数指针。
    u8 (*pfvidTrf)(u8 u8CurSta); //!< 状态转换函数指针。
    void (*pfvidEx)(void);       //!< 退出状态函数指针。
} stStaCb;

/**
 * @struct stSta
 * @brief 状态机句柄结构体。
 * @details 持有状态回调表指针、当前状态、初始状态及状态个数。
 */
typedef struct stSta
{
    const stStaCb* pktCbTbl; //!< 状态回调表指针。
    u8 u8CurSta;             //!< 当前状态。
    u8 u8StaAmt;             //!< 状态个数。
} stSta;

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化状态机。
 * @details 绑定回调表与状态个数，将状态复位为初始状态（枚举值 0），
 *          随后调用初始状态的进入回调。
 * @param[in] kpktSta 状态机句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_NUL_PTR 句柄或回调表为空指针。
 * @retval ER_SW_INV_PARAM 状态个数非法（不大于 0）。
 */
err erInitSta(const stSta* const kpktSta);

/**
 * @brief 状态机周期处理函数。
 * @details 每个调用周期内依次完成：查询当前状态的转换条件；若发生转换，
 *          则退出旧状态并进入新状态；最后运行当前状态。
 * @param[in, out] kptSta 状态机句柄。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 状态转换函数返回了越界状态，本次不切换状态。
 */
err erTckSta(stSta* const kptSta);

/**
 * @brief 获取当前状态。
 * @param[in] kpktSta 状态机句柄。
 * @param[out] kpu8CurSta 当前状态枚举值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err eGetCurSta(const stSta* const kpktSta, u8* const kpu8CurSta);

#ifdef __cplusplus
}
#endif

#endif // STA_H
