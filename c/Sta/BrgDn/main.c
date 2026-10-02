/**
 * @file Main.c
 * @brief 下电启动示例程序。
 * @details 演示下电启动状态机的初始化与周期处理。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include <unistd.h>
#include "BrgDn.h"

/**
 * @brief 延时 1 秒。
 */
static void vidDelay1s(void)
{
    sleep(1);
}

int main(void)
{
    enBrgDn eCurSta = BRG_DN_CTL_STP;

    // 初始化状态机。
    erInitBrgDn();

    while (1)
    {
        // 处理状态机。
        erTckBrgDn();

        eGetCurBrgDn(&eCurSta);
        printf("Current State: %d\n", eCurSta); // 输出当前状态。

        // 延时 1 秒。
        vidDelay1s();
    }

    return 0;
}
