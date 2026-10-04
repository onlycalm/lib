/**
 * @file Main.c
 * @brief DTC 示例程序。
 * @details 测试 DevDtc 模块的对外接口：初始化、故障上报、操作循环结束评估与
 *          开始复位、清除、清除全部、掉电保存/恢复，以及状态/发生次数/时间/
 *          快照/编号查询。
 * @author Calm
 * @data 2026-10-04
 * @version v1.0.0
 * @copyright Calm
 */

#include "Com.h"
#include "DevDtc.h"

#include <stdio.h>

/**
 * @brief 打印单个 DTC 的编号、状态、发生次数与时间。
 * @details 依次调用 eGetDevDtcCd / eGetDevDtcSts / eGetDevDtcCnt /
 *          eGetDevDtcTm 并打印结果。
 * @param[in] eId DTC 枚举。
 */
static void vidPrnDtc(enDevDtc eId)
{
    u16 u16Cd = 0u;
    u32 u32Cnt = 0u;
    u64 u64FstTm = 0u;
    u64 u64LstTm = 0u;
    unDtcSts unSts;

    eGetDevDtcCd(eId, &u16Cd);
    eGetDevDtcSts(eId, &unSts);
    eGetDevDtcCnt(eId, &u32Cnt);
    eGetDevDtcTm(eId, &u64FstTm, &u64LstTm);

    // 字段含义：编号(0x%04X)、sts 状态字节、cur 当前故障、pend 待定、
    // cfm 已确认、cnt 发生次数、fst 首次时间、lst 最近时间。
    printf("0x%04X: sts=0x%02X(cur=%u pend=%u cfm=%u) cnt=%u fst=%llu lst=%llu\n",
           (unsigned)u16Cd, (unsigned)unSts.u8Dat, (unsigned)unSts.btTstFld,
           (unsigned)unSts.btPend, (unsigned)unSts.btCfm,
           (unsigned)u32Cnt, (unsigned long long)u64FstTm,
           (unsigned long long)u64LstTm);
}

int main(void)
{
    stDevDtcSnp tSnp;

    // 测试 erInitDevDtc：初始化。
    erInitDevDtc();

    // 测试 erReportDevDtc(TRUE)：上报故障置位。
    printf("=== 1. 上报温度过高故障（置位） ===\n");
    erReportDevDtc(DEV_DTC_TEMP_HIGH, TRUE);
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    // 测试 eGetDevDtcSnp：查询冻结帧快照。
    printf("=== 2. 查询冻结帧快照 ===\n");
    eGetDevDtcSnp(DEV_DTC_TEMP_HIGH, &tSnp);
    printf("snapshot: ch=%u temp=%u volt=%u\n", (unsigned)tSnp.u8Ch,
           (unsigned)tSnp.u16Temp, (unsigned)tSnp.u16Volt);

    // 测试 erEndCycDevDtc + erNewCycDevDtc：连续 2 循环失败 -> 确认。
    printf("=== 3. 连续 2 个循环失败 -> 确认 ===\n");
    erEndCycDevDtc();  // 结束循环 1：评估（失败，失败计数 1）。
    erNewCycDevDtc();  // 开始循环 2。
    erReportDevDtc(DEV_DTC_TEMP_HIGH, TRUE);
    erEndCycDevDtc();  // 结束循环 2：评估（失败，失败计数 2 -> 确认）。
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    // 测试 erReportDevDtc(FALSE) + erEndCycDevDtc + erNewCycDevDtc：治愈。
    printf("=== 4. 故障恢复，连续 2 个循环正常 -> 治愈 ===\n");
    erNewCycDevDtc();  // 开始循环 3。
    erReportDevDtc(DEV_DTC_TEMP_HIGH, FALSE);
    erEndCycDevDtc();  // 结束循环 3：评估（正常，治愈计数 1）。
    erNewCycDevDtc();  // 开始循环 4。
    erEndCycDevDtc();  // 结束循环 4：评估（正常，治愈计数 2 -> 治愈）。
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    // 测试掉电持久化：erSaveDevDtc + erInitDevDtc + erLoadDevDtc。
    printf("=== 5. 掉电持久化（保存/复位/恢复跨循环确认） ===\n");
    erReportDevDtc(DEV_DTC_TEMP_HIGH, TRUE);
    erEndCycDevDtc();  // 结束循环：失败计数 1。
    erSaveDevDtc();    // 掉电前保存记录。
    printf("[掉电前保存] ");
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    erInitDevDtc();    // 模拟复位：内存清空。
    printf("[复位后清空] ");
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    erLoadDevDtc();    // 上电恢复记录。
    erNewCycDevDtc();  // 开始新循环。
    printf("[上电恢复]   ");
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    erReportDevDtc(DEV_DTC_TEMP_HIGH, TRUE);
    erEndCycDevDtc();  // 结束循环：失败计数 2 -> 确认（跨复位累计）。
    printf("[跨复位确认] ");
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    // 测试快照每个故障沿覆盖。
    printf("=== 6. 快照每次故障沿覆盖 ===\n");
    erReportDevDtc(DEV_DTC_VOLT_LOW, TRUE);
    eGetDevDtcSnp(DEV_DTC_VOLT_LOW, &tSnp);
    printf("第1次快照: temp=%u\n", (unsigned)tSnp.u16Temp);
    erReportDevDtc(DEV_DTC_VOLT_LOW, FALSE);
    erReportDevDtc(DEV_DTC_VOLT_LOW, TRUE);
    eGetDevDtcSnp(DEV_DTC_VOLT_LOW, &tSnp);
    printf("第2次快照: temp=%u\n", (unsigned)tSnp.u16Temp);

    // 测试 erClrDevDtc：清除单个 DTC。
    printf("=== 7. 再次故障后清除单个 ===\n");
    erReportDevDtc(DEV_DTC_TEMP_HIGH, TRUE);
    erClrDevDtc(DEV_DTC_TEMP_HIGH);
    vidPrnDtc(DEV_DTC_TEMP_HIGH);

    // 测试 erClrAllDevDtc：清除全部 DTC。
    printf("=== 8. 上报两个故障后清除全部 ===\n");
    erReportDevDtc(DEV_DTC_VOLT_LOW, TRUE);
    erReportDevDtc(DEV_DTC_COMM_LOST, TRUE);
    erClrAllDevDtc();
    vidPrnDtc(DEV_DTC_VOLT_LOW);
    vidPrnDtc(DEV_DTC_COMM_LOST);

    return 0;
}
