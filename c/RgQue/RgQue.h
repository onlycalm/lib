/**
 * @file RgQue.h
 * @brief 环形队列框架。
 * @details 定义环形队列句柄结构体及 API。框架本身不持有缓冲区，缓冲区由实例层
 *          静态提供，句柄仅记录基地址、容量与读写字节偏移。
 *          队列按字节流方式存取，读写索引以字节为单位，天然支持变长数据；
 *          帧的定界与长度解析由上层协议层负责。
 *          采用「预留一个空位」的满/空判定方案，读索引仅由消费侧修改、写索引
 *          仅由生产侧修改，可支持单生产者/单消费者（SPSC）无锁共享。
 *          框架为可移植模块，支持多实例：每个队列持有独立句柄 stRgQue。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef RG_QUE_H
#define RG_QUE_H

#include "Typ.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ===== 结构体定义 ===== */
/**
 * @struct stRgQue
 * @brief 环形队列句柄结构体。
 * @details 持有缓冲区基地址、容量及读写字节偏移。缓冲区内存由实例层提供，
 *          框架不负责分配与释放。读写偏移以字节为单位。
 */
typedef struct stRgQue
{
    u8* pu8Buf;   //!< 缓冲区基地址（连续字节数组）。
    u16 u16Cap;   //!< 容量（字节数），至少为 2。
    u16 u16RdIdx; //!< 读索引（字节偏移，队头）。
    u16 u16WrIdx; //!< 写索引（字节偏移，队尾）。
} stRgQue;

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化环形队列。
 * @details 校验句柄参数并将读写索引复位为 0（清空队列）。
 * @param[in, out] kptQue 环形队列句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 缓冲区为空或容量小于 2。
 */
err erInitRgQue(stRgQue* const kptQue);

/**
 * @brief 入队若干字节。
 * @details 空间不足时整批拒绝，不写入任何字节（原子性）。
 * @param[in, out] kptQue 环形队列句柄。
 * @param[in] kpvDat 待写入数据指针。
 * @param[in] ku16Amt 待写入字节数。
 * @return 入队结果。
 * @retval ER_SUC 入队成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待写入字节数为 0。
 * @retval ER_DAT_FUL 剩余空间不足。
 */
err erPshRgQue(stRgQue* const kptQue, const void* const kpvDat,
               const u16 ku16Amt);

/**
 * @brief 出队若干字节。
 * @details 数据不足时整批拒绝，不读出任何字节（原子性）。
 * @param[in, out] kptQue 环形队列句柄。
 * @param[out] pvDat 读出数据指针。
 * @param[in] ku16Amt 待读出字节数。
 * @return 出队结果。
 * @retval ER_SUC 出队成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待读出字节数为 0。
 * @retval ER_DAT_EMPTY 数据不足。
 */
err erPopRgQue(stRgQue* const kptQue, void* const pvDat, const u16 ku16Amt);

/**
 * @brief 窥看队头若干字节（不出队）。
 * @param[in] kpktQue 环形队列句柄。
 * @param[out] pvDat 读出数据指针。
 * @param[in] ku16Amt 待窥看字节数。
 * @return 窥看结果。
 * @retval ER_SUC 窥看成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或数据指针为空。
 * @retval ER_SW_INV_PARAM 待窥看字节数为 0。
 * @retval ER_DAT_EMPTY 数据不足。
 */
err erPkRgQue(const stRgQue* const kpktQue, void* const pvDat,
              const u16 ku16Amt);

/**
 * @brief 获取已用字节数。
 * @param[in] kpktQue 环形队列句柄。
 * @return 已用字节数。
 * @retval 0 句柄为空或队列为空。
 */
u16 u16GetUsedRgQue(const stRgQue* const kpktQue);

/**
 * @brief 获取可用字节数。
 * @param[in] kpktQue 环形队列句柄。
 * @return 可用字节数。
 * @retval 0 句柄为空或队列已满。
 */
u16 u16GetFreeRgQue(const stRgQue* const kpktQue);

/**
 * @brief 判断队列是否为空。
 * @param[in] kpktQue 环形队列句柄。
 * @return 判断结果。
 * @retval TRUE 队列为空。
 * @retval FALSE 队列不为空或句柄为空。
 */
bl bEmptyRgQue(const stRgQue* const kpktQue);

/**
 * @brief 判断队列是否已满。
 * @param[in] kpktQue 环形队列句柄。
 * @return 判断结果。
 * @retval TRUE 队列已满。
 * @retval FALSE 队列未满或句柄为空。
 */
bl bFullRgQue(const stRgQue* const kpktQue);

/**
 * @brief 清空环形队列。
 * @details 将读写索引复位为 0。
 * @param[in, out] kptQue 环形队列句柄。
 * @return 清空结果。
 * @retval ER_SUC 清空成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 */
err erResetRgQue(stRgQue* const kptQue);

#ifdef __cplusplus
}
#endif

#endif // RG_QUE_H
