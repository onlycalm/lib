/**
 * @file Main.c
 * @brief 模拟量监控示例程序。
 * @details 演示模拟量监控器的初始化与周期处理：先模拟上电阶段（ADC 未就绪、
 *          不误报），再让母线电压爬升越过上限触发故障、回落恢复正常触发恢复。
 * @author Calm
 * @data 2026-10-05
 * @version v1.0.0
 * @copyright Calm
 */

#include "Com.h"
#include "DevMon.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief 延时 100 毫秒。
 */
static void vidDelay100ms(void)
{
    usleep(100000);
}

int main(void)
{
    u16 u16Step = 0u;
    bl blFlt = FALSE;
    s32 s32Volt = 0;

    // 初始化监控器。
    erInitDevMon();

    // 模拟上电阶段：ADC 未就绪，各通道默认值 0，不应误报下限故障。
    for(u16Step = 0u; u16Step < 3u; u16Step++)
    {
        erTckDevMon();
        eGetDevMonFlt(DEV_MON_RULE_BUS_VOLT_DN, &blFlt);
        eGetDevMonVal(DEV_MON_CH_BUS_VOLT, &s32Volt);

        printf("boot step=%u volt=%ld dnFlt=%u\n",
               (unsigned)u16Step, (long)s32Volt, (unsigned)blFlt);

        vidDelay100ms();
    }

    // ADC 就绪，采集模块写入各通道正常值。
    erSetDevMonVal(DEV_MON_CH_BUS_VOLT, 48000); // 48.000V。
    erSetDevMonVal(DEV_MON_CH_BUS_CUR, 0);      // 0.000A。
    erSetDevMonVal(DEV_MON_CH_TEMP, 25000);     // 25.000℃。

    // 母线电压先爬升越过上限触发故障，再回落恢复正常触发恢复。
    for(u16Step = 0u; u16Step < 20u; u16Step++)
    {
        if(u16Step < 8u)
        {
            s32Volt = 48000 + (s32)u16Step * 1000;
        }
        else
        {
            s32Volt = 55000 - (s32)(u16Step - 8u) * 1000;
        }

        erSetDevMonVal(DEV_MON_CH_BUS_VOLT, s32Volt);
        erTckDevMon();
        eGetDevMonFlt(DEV_MON_RULE_BUS_VOLT_UP, &blFlt);

        printf("step=%u volt=%ld flt=%u\n",
               (unsigned)u16Step, (long)s32Volt, (unsigned)blFlt);

        vidDelay100ms();
    }

    return 0;
}
