/**
 * @file NetRx.c
 * @brief 以太网接收缓存实例实现。
 * @details 定义接收字节缓冲区与队列句柄，封装框架 API。本文件为具体项目的
 *          差异化配置，随项目不同而修改。帧的定界与长度解析由上层协议层负责。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#include "NetRx.h"
#include "RgQue.h"

/* ===== 变量定义 ===== */
/* == 静态变量 == */
/* 接收字节缓冲区。 */
static u8 s_au8NetRxBuf[NET_RX_BUF_SZ];

/* 接收缓存句柄。 */
static stRgQue s_tNetRx = {
    .pu8Buf = s_au8NetRxBuf,
    .u16Cap = NET_RX_BUF_SZ,
    .u16RdIdx = 0u,
    .u16WrIdx = 0u,
};

/* ===== 函数定义 ===== */
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
err erInitNetRx(void)
{
    return erInitRgQue(&s_tNetRx);
}

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
err erPshNetRx(const u8* const kpu8Dat, const u16 ku16Len)
{
    return erPshRgQue(&s_tNetRx, kpu8Dat, ku16Len);
}

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
err erPopNetRx(u8* const pu8Dat, const u16 ku16Len)
{
    return erPopRgQue(&s_tNetRx, pu8Dat, ku16Len);
}

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
err erPkNetRx(u8* const pu8Dat, const u16 ku16Len)
{
    return erPkRgQue(&s_tNetRx, pu8Dat, ku16Len);
}

/**
 * @brief 获取接收缓存已用字节数。
 * @return 已用字节数。
 * @retval 0 接收缓存为空。
 */
u16 u16GetUsedNetRx(void)
{
    return u16GetUsedRgQue(&s_tNetRx);
}

/**
 * @brief 判断接收缓存是否为空。
 * @return 判断结果。
 * @retval TRUE 接收缓存为空。
 * @retval FALSE 接收缓存不为空。
 */
bl bEmptyNetRx(void)
{
    return bEmptyRgQue(&s_tNetRx);
}

/**
 * @brief 判断接收缓存是否已满。
 * @return 判断结果。
 * @retval TRUE 接收缓存已满。
 * @retval FALSE 接收缓存未满。
 */
bl bFullNetRx(void)
{
    return bFullRgQue(&s_tNetRx);
}
