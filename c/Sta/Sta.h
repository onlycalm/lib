/**
 * @file Sta.h
 * @brief 状态机框架。
 * @details 定义状态回调结构体、状态回调表的外部声明及状态机 API。框架本身不含任何具体状态，具体状态与回调表由 StaImpl 提供。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef STA_H
#define STA_H

#include "Typ.h"
#include "StaImpl.h"

/* ===== 结构体定义 ===== */
/**
 * @struct stStaCb
 * @brief 状态回调函数指针结构体。
 * @details 描述单个状态在进入、运行、转换、退出四个阶段的回调。
 */
typedef struct stStaCb
{
    void (*pfvidEnt)(void); //!< 进入状态函数指针。
    void (*pfvidRun)(void); //!< 运行状态函数指针。
    ESta (*pfvidTrf)(void); //!< 状态转换函数指针。
    void (*pfvidEx)(void);  //!< 退出状态函数指针。
} stStaCb;

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化状态机。
 * @return 初始化结果。
 * @retval EC_OK 初始化成功。
 */
extern err erInitSta(void);

/**
 * @brief 状态机周期处理函数。
 * @return 处理结果。
 * @retval EC_OK 处理成功。
 */
extern err erTckSta(void);

/**
 * @brief 获取当前状态。
 * @return 当前状态枚举。
 */
extern ESta eGetCurSta(void);

#endif //STA_H
