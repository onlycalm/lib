/**
 * @file Main.c
 * @brief 上电启动示例程序。
 * @details 演示上电启动状态机的初始化与周期处理。
 * @author Calm
 * @data 2026-10-03
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include <unistd.h>
#include "BrgUp.h"

/**
 * @brief 延时 1 秒。
 */
static void vidDelay1s(void)
{
    sleep(1);
}

int main(void)
{
    enBrgUp eCurSta = BRG_UP_CHP_INIT;

    // 初始化状态机。
    erInitBrgUp();

    while (1)
    {
        // 处理状态机。
        erTckBrgUp();

        eGetCurBrgUp(&eCurSta);
        printf("Current State: %d\n", eCurSta); // 输出当前状态。

        // 延时 1 秒。
        vidDelay1s();
    }

    return 0;
}
