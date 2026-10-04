/**
 * @file NetRx.h
 * @brief 以太网接收缓存实例。
 * @details 定义接收缓存字节数、单帧最大字节数及接收缓存 API。本文件为具体项目
 *          的差异化配置，框架部分（RgQue）无需修改。
 *          接收缓存按字节流存取，帧长由上层协议解析帧头长度字段得到。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef NET_RX_H
#define NET_RX_H

#include "Typ.h"

/* ===== 宏定义 ===== */
#define NET_RX_FRAME_SZ  1024u //!< 单帧最大字节数。
#define NET_RX_FRAME_AMT 10u   //!< 缓存可容纳的最大帧数。
// 预留一个空位用于满/空判定（+1），故总字节数 = 帧数 * 帧长 + 1。
#define NET_RX_BUF_SZ    (NET_RX_FRAME_AMT * NET_RX_FRAME_SZ + 1u)

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化接收缓存。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄为空指针。
 * @retval ER_SW_INV_PARAM 缓冲区为空或容量小于 2。
 */
err erInitNetRx(void);

/**
 * @brief 写入一帧数据到接收缓存。
 * @param[in] kpu8Dat 待写入数据指针。
 * @param[in] ku16Len 待写入字节数（帧长）。
 * @return 写入结果。
 * @retval ER_SUC 写入成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 数据指针为空。
 * @retval ER_SW_INV_PARAM 待写入字节数为 0。
 * @retval ER_DAT_FUL 接收缓存空间不足。
 */
err erPshNetRx(const u8* const kpu8Dat, const u16 ku16Len);

/**
 * @brief 从接收缓存取出一帧数据。
 * @param[out] pu8Dat 读出数据指针。
 * @param[in] ku16Len 待读出字节数（帧长）。
 * @return 取出结果。
 * @retval ER_SUC 取出成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 数据指针为空。
 * @retval ER_SW_INV_PARAM 待读出字节数为 0。
 * @retval ER_DAT_EMPTY 接收缓存数据不足。
 */
err erPopNetRx(u8* const pu8Dat, const u16 ku16Len);

/**
 * @brief 窥看接收缓存队头数据（不出队）。
 * @param[out] pu8Dat 读出数据指针。
 * @param[in] ku16Len 待窥看字节数。
 * @return 窥看结果。
 * @retval ER_SUC 窥看成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 数据指针为空。
 * @retval ER_SW_INV_PARAM 待窥看字节数为 0。
 * @retval ER_DAT_EMPTY 接收缓存数据不足。
 */
err erPkNetRx(u8* const pu8Dat, const u16 ku16Len);

/**
 * @brief 获取接收缓存已用字节数。
 * @return 已用字节数。
 * @retval 0 接收缓存为空。
 */
u16 u16GetUsedNetRx(void);

/**
 * @brief 判断接收缓存是否为空。
 * @return 判断结果。
 * @retval TRUE 接收缓存为空。
 * @retval FALSE 接收缓存不为空。
 */
bl bEmptyNetRx(void);

/**
 * @brief 判断接收缓存是否已满。
 * @return 判断结果。
 * @retval TRUE 接收缓存已满。
 * @retval FALSE 接收缓存未满。
 */
bl bFullNetRx(void);

#endif // NET_RX_H
