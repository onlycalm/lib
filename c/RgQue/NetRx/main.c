/**
 * @file main.c
 * @brief 以太网接收缓存示例程序。
 * @details 覆盖 NetRx 各接口，并校验缓存数据写入与读出后保持一致：
 *          用可识别模式填充帧数据，写入后窥看/读出并逐字节比对。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include "Com.h"
#include "NetRx.h"

/**
 * @brief 用可识别模式填充数据（模式 = 种子 + 下标，模 256）。
 * @param[out] pu8Dat 待填充缓冲。
 * @param[in] ku16Len 字节数。
 * @param[in] ku8Seed 模式种子。
 */
static void vidFill(u8* const pu8Dat, const u16 ku16Len, const u8 ku8Seed)
{
    u16 u16i = 0u;

    for(u16i = 0u; u16i < ku16Len; u16i++)
    {
        pu8Dat[u16i] = (u8)(ku8Seed + u16i);
    }
}

/**
 * @brief 校验数据与填充模式一致。
 * @param[in] kpu8Dat 待校验数据。
 * @param[in] ku16Len 字节数。
 * @param[in] ku8Seed 模式种子。
 * @return 校验结果。
 * @retval TRUE 一致。
 * @retval FALSE 不一致。
 */
static bl bChk(const u8* const kpu8Dat, const u16 ku16Len, const u8 ku8Seed)
{
    u16 u16i = 0u;

    for(u16i = 0u; u16i < ku16Len; u16i++)
    {
        if(kpu8Dat[u16i] != (u8)(ku8Seed + u16i))
        {
            return FALSE;
        }
    }

    return TRUE;
}

int main(void)
{
    u8 au8Wr[NET_RX_FRAME_SZ]; // 写入缓冲。
    u8 au8Rd[NET_RX_FRAME_SZ]; // 读出缓冲。
    u16 u16Len = 0u;
    u16 u16i = 0u;
    u16 u16Bad = 0u;

    /* 1. 初始化。 */
    erInitNetRx();
    printf("[init] empty=%u used=%u\n\n",
           (unsigned)bEmptyNetRx(), (unsigned)u16GetUsedNetRx());

    /* 2. 写入 3 帧变长数据，帧内容用不同种子区分。 */
    for(u16i = 0u; u16i < 3u; u16i++)
    {
        u16Len = 64u + u16i * 64u; // 64 / 128 / 192 字节。

        vidFill(au8Wr, u16Len, (u8)u16i);
        erPshNetRx(au8Wr, u16Len);
    }
    printf("[push 3 frames] used=%u\n\n",
           (unsigned)u16GetUsedNetRx());

    /* 3. 窥看第一帧并校验，验证窥看后数据未出队。 */
    erPkNetRx(au8Rd, 64u);
    printf("[peek frame0] data=%s used=%u(unchanged)\n\n",
           bChk(au8Rd, 64u, 0u) ? "OK" : "BAD",
           (unsigned)u16GetUsedNetRx());

    /* 4. 逐帧取出并校验数据。 */
    for(u16i = 0u; u16i < 3u; u16i++)
    {
        u16Len = 64u + u16i * 64u;

        erPopNetRx(au8Rd, u16Len);
        printf("[pop frame%u len=%u] data=%s\n",
               (unsigned)u16i, (unsigned)u16Len,
               bChk(au8Rd, u16Len, (u8)u16i) ? "OK" : "BAD");
    }
    printf("[after pop all] empty=%u used=%u\n\n",
           (unsigned)bEmptyNetRx(), (unsigned)u16GetUsedNetRx());

    /* 5. 填满缓存（10 个最大帧），再逐帧取出并校验，覆盖跨边界回绕。 */
    for(u16i = 0u; u16i < NET_RX_FRAME_AMT; u16i++)
    {
        vidFill(au8Wr, NET_RX_FRAME_SZ, (u8)u16i);
        erPshNetRx(au8Wr, NET_RX_FRAME_SZ);
    }
    printf("[fill %u max frames] full=%u used=%u\n",
           (unsigned)NET_RX_FRAME_AMT,
           (unsigned)bFullNetRx(), (unsigned)u16GetUsedNetRx());

    u16Bad = 0u;
    for(u16i = 0u; u16i < NET_RX_FRAME_AMT; u16i++)
    {
        erPopNetRx(au8Rd, NET_RX_FRAME_SZ);

        if(bChk(au8Rd, NET_RX_FRAME_SZ, (u8)u16i) == FALSE)
        {
            u16Bad++;
        }
    }
    printf("[pop %u max frames] data=%s empty=%u used=%u\n",
           (unsigned)NET_RX_FRAME_AMT,
           (u16Bad == 0u) ? "OK" : "BAD",
           (unsigned)bEmptyNetRx(), (unsigned)u16GetUsedNetRx());

    return 0;
}
