/**
 * @file main.c
 * @brief 状态机示例程序。
 * @details 演示状态机引擎的初始化与周期处理。
 * @author Calm
 * @data 2026-09-26
 * @version v1.0.0
 * @copyright Calm
 */

#include "Sta.h"

#include <unistd.h>

/**
 * @brief 延时 1 秒。
 */
static void vidDelay1s(void)
{
    sleep(1);
}

int main(void)
{
    // 初始化状态机。
    erInitSta();

    while (1)
    {
        // 处理状态机。
        erTckSta();

        // 延时 1 秒。
        vidDelay1s();
    }

    return 0;
}
