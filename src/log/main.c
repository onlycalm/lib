/**
 * @file main.c
 * @brief Log模块测试程序。
 * @details 无
 * @author Calm
 * @date 2026-09-25
 * @version v1.0.0
 * @copyright Calm
 */

#include "log.h"

/**
 * @brief Log模块测试入口。
 * @details 初始化日志模块后，依次输出各等级的日志，验证日志输出功能。
 * @return 退出码。
 * @retval 0 正常退出。
 */
int main(void)
{
    erInitLog("log.txt");
    LogCrt("This is a critical log.");
    LogErr("This is an error log.");
    LogWrn("This is a warning log.");
    LogScs("This is a success log.");
    LogInf("This is an info log.");
    LogDbg("This is a debug log.");
    LogTr("This is a trace log.");
}
