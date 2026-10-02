/**
 * @file Main.c
 * @brief 状态机示例程序。
 * @details 演示状态机引擎的初始化与周期处理。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#include <stdio.h>
#include <unistd.h>
#include "DevSta.h"

/**
 * @brief 延时 1 秒。
 */
static void vidDelay1s(void)
{
    sleep(1);
}

int main(void)
{
    enDevSta eCurSta = DEV_STA_INIT;

    // 初始化状态机。
    erInitDevSta();

    while (1)
    {
        // 处理状态机。
        erTckDevSta();

        eGetCurDevSta(&eCurSta);
        printf("Current State: %d\n", eCurSta); // 输出当前状态。

        // 延时 1 秒。
        vidDelay1s();
    }

    return 0;
}
