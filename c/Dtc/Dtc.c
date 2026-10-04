/**
 * @file Dtc.c
 * @brief DTC 诊断故障码框架实现。
 * @details 实现 DTC 管理器的初始化、上报、操作循环推进、清除、查询与保存/恢复。
 *          框架不定义具体 DTC，通过句柄绑定的记录与配置数组驱动，支持多实例。
 * @author Calm
 * @data 2026-10-04
 * @version v1.0.0
 * @copyright Calm
 */

#include "Dtc.h"
#include "Typ.h"
#include <string.h>
#define ER_DOM     ER_DOM_LIB
#define ER_SUB_DOM ER_SUB_DOM_LIB_C
#define ER_MOD     ER_MOD_DTC
#include "Er.h"

/* ===== 静态函数声明 ===== */
/**
 * @brief 清除单条 DTC 记录的历史状态。
 * @param[in, out] ptRec DTC 记录指针。
 */
static void vidClrRec(stDtcRec* const ptRec);

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化 DTC 管理器。
 * @details 校验句柄参数并将全部记录的运行时状态清零，不修改配置。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或配置数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 数量为 0。
 */
err erInitDtc(stDtc* const kptDtc)
{
    u16 u16Idx = 0u;
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL) || (kptDtc->pktCfg == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(kptDtc->u16Amt == 0u)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        for(u16Idx = 0u; u16Idx < kptDtc->u16Amt; u16Idx++)
        {
            kptDtc->ptRec[u16Idx].unSts.u8Dat = 0u;
            kptDtc->ptRec[u16Idx].u8FldCyc = 0u;
            kptDtc->ptRec[u16Idx].u8PassCyc = 0u;
            kptDtc->ptRec[u16Idx].u32OcCnt = 0u;
            kptDtc->ptRec[u16Idx].u64FstTm = 0u;
            kptDtc->ptRec[u16Idx].u64LstTm = 0u;
            kptDtc->ptRec[u16Idx].blSnpVld = FALSE;
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 上报单个 DTC 的当前检测结果。
 * @details TRUE 表示当前检测失败，FALSE 表示检测通过。每次上报 TRUE 均置位
 *          本操作循环失败锁存；检测失败沿（由正常转故障）会累计发生次数、
 *          更新时间并捕获快照（覆盖为最近一次）；重复置位幂等。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[in] kbAct 当前检测结果（TRUE 故障 / FALSE 正常）。
 * @return 上报结果。
 * @retval ER_SUC 上报成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erReportDtc(stDtc* const kptDtc, const u16 ku16Id, const bl kbAct)
{
    stDtcRec* ptRec = NULL;
    u64 u64Now = 0u;
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kptDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        ptRec = &kptDtc->ptRec[ku16Id];

        if(kbAct == TRUE)
        {
            // 本操作循环故障锁存：每次上报故障均置位，跨循环持续有效，
            // 仅在新操作循环时由 erNewCycDtc 复位。
            ptRec->unSts.btTstFldCyc = 1u;

            // 检测失败沿：由未故障转为故障。
            if(ptRec->unSts.btTstFld == 0u)
            {
                ptRec->unSts.btPend = 1u;
                ptRec->unSts.btTstFldClr = 1u;

                // 发生次数 +1（饱和）。
                if(ptRec->u32OcCnt < 0xFFFFFFFFu)
                {
                    ptRec->u32OcCnt++;
                }

                // 更新时间戳（时间源未配置则跳过）。
                if(kptDtc->pfu64GetTm != NULL)
                {
                    u64Now = kptDtc->pfu64GetTm();
                    ptRec->u64LstTm = u64Now;

                    if(ptRec->u32OcCnt == 1u)
                    {
                        ptRec->u64FstTm = u64Now;
                    }
                }

                // 每个失败沿捕获快照（覆盖为最近一次）。
                if((kptDtc->pktCfg[ku16Id].pu8Snp != NULL) &&
                   (kptDtc->pfvidSnp != NULL))
                {
                    kptDtc->pfvidSnp(ku16Id, kptDtc->pktCfg[ku16Id].pu8Snp);
                    ptRec->blSnpVld = TRUE;
                }
            }

            ptRec->unSts.btTstFld = 1u;
        }
        else
        {
            ptRec->unSts.btTstFld = 0u;
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 结束当前操作循环并评估。
 * @details 根据本循环是否失败，逐 DTC 推进成熟/治愈去抖计数，并在达到阈值时
 *          确认（confirmed）或治愈清除。不复位每循环锁存位，通常在掉电前调用，
 *          配合 erSaveDtc 保存。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erEndCycDtc(stDtc* const kptDtc)
{
    u16 u16Idx = 0u;
    stDtcRec* ptRec = NULL;
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        for(u16Idx = 0u; u16Idx < kptDtc->u16Amt; u16Idx++)
        {
            ptRec = &kptDtc->ptRec[u16Idx];

            if(ptRec->unSts.btTstFldCyc == 1u)
            {
                // 本循环失败过：推进确认去抖。
                if(ptRec->u8FldCyc < 0xFFu)
                {
                    ptRec->u8FldCyc++;
                }
                ptRec->u8PassCyc = 0u;

                if((ptRec->unSts.btCfm == 0u) &&
                   (ptRec->u8FldCyc >= kptDtc->u8CfmCyc))
                {
                    ptRec->unSts.btCfm = 1u; // 确认历史故障。
                    ptRec->unSts.btPend = 0u;
                }
            }
            else if(ptRec->unSts.btCfm == 1u)
            {
                // 已确认且本循环未失败：推进治愈去抖。
                if(ptRec->u8PassCyc < 0xFFu)
                {
                    ptRec->u8PassCyc++;
                }

                if(ptRec->u8PassCyc >= kptDtc->u8HealCyc)
                {
                    ptRec->unSts.btCfm = 0u; // 治愈清除历史故障。
                    ptRec->unSts.btPend = 0u;
                    ptRec->u8FldCyc = 0u;
                    ptRec->u8PassCyc = 0u;
                }
            }
            else
            {
                // 未确认且本循环未失败：复位确认去抖与待定位。
                ptRec->u8FldCyc = 0u;
                ptRec->unSts.btPend = 0u;
            }
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 开始一个新操作循环。
 * @details 复位每循环锁存位（本操作循环失败、本操作循环测试未完成），
 *          通常在上电后调用，配合 erLoadDtc 恢复。成熟/治愈评估由 erEndCycDtc
 *          在循环结束时完成。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erNewCycDtc(stDtc* const kptDtc)
{
    u16 u16Idx = 0u;
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        for(u16Idx = 0u; u16Idx < kptDtc->u16Amt; u16Idx++)
        {
            // 复位每操作循环位。
            kptDtc->ptRec[u16Idx].unSts.btTstFldCyc = 0u;
            kptDtc->ptRec[u16Idx].unSts.btTstNtCplCyc = 0u;
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 清除单个 DTC。
 * @details 对应 UDS ClearDiagnosticInformation 服务：清 confirmed/pending/
 *          testFailedSinceLastClear 及发生次数、时间、快照与去抖计数，
 *          恢复为未发生态；不改变当前实时故障状态。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erClrDtc(stDtc* const kptDtc, const u16 ku16Id)
{
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kptDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        vidClrRec(&kptDtc->ptRec[ku16Id]);
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 清除全部 DTC。
 * @details 对应 UDS 按组清除服务：逐条清除所有 DTC 的历史记录，恢复为未发生态；
 *          不改变各 DTC 的当前实时故障状态。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erClrAllDtc(stDtc* const kptDtc)
{
    u16 u16Idx = 0u;
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        for(u16Idx = 0u; u16Idx < kptDtc->u16Amt; u16Idx++)
        {
            vidClrRec(&kptDtc->ptRec[u16Idx]);
        }

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个 DTC 的状态字节。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[out] kptSts 状态字节指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或状态指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDtcSts(const stDtc* const kpktDtc, const u16 ku16Id,
               unDtcSts* const kptSts)
{
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->ptRec == NULL) || (kptSts == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kpktDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        kptSts->u8Dat = kpktDtc->ptRec[ku16Id].unSts.u8Dat;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个 DTC 的发生次数。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[out] kpu32Cnt 发生次数指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或次数指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDtcCnt(const stDtc* const kpktDtc, const u16 ku16Id,
               u32* const kpu32Cnt)
{
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->ptRec == NULL) || (kpu32Cnt == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kpktDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        *kpu32Cnt = kpktDtc->ptRec[ku16Id].u32OcCnt;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个 DTC 的首次/最近发生时间。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[out] kpu64FstTm 首次发生时间指针。
 * @param[out] kpu64LstTm 最近发生时间指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或时间指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDtcTm(const stDtc* const kpktDtc, const u16 ku16Id,
              u64* const kpu64FstTm, u64* const kpu64LstTm)
{
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->ptRec == NULL) || (kpu64FstTm == NULL) ||
       (kpu64LstTm == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kpktDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        *kpu64FstTm = kpktDtc->ptRec[ku16Id].u64FstTm;
        *kpu64LstTm = kpktDtc->ptRec[ku16Id].u64LstTm;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个 DTC 的冻结帧快照。
 * @details 拷贝快照缓冲区至调用方，拷贝量为入参字节数与快照大小中较小者。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[out] kpvDat 快照读出指针。
 * @param[in] ku16Amt 待读出字节数。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或读出指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界或待读出字节数为 0。
 * @retval ER_SW_NOT_SUP 该 DTC 未配置快照缓冲区。
 * @retval ER_DAT_EMPTY 快照尚未捕获。
 */
err eGetDtcSnp(const stDtc* const kpktDtc, const u16 ku16Id,
               void* const kpvDat, const u16 ku16Amt)
{
    u16 u16Cpy = 0u;
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->ptRec == NULL) ||
       (kpktDtc->pktCfg == NULL) || (kpvDat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if((ku16Id >= kpktDtc->u16Amt) || (ku16Amt == 0u))
    {
        erRet = ER_SW_INV_PARAM;
    }
    else if(kpktDtc->pktCfg[ku16Id].pu8Snp == NULL)
    {
        erRet = ER_SW_NOT_SUP;
    }
    else if(kpktDtc->ptRec[ku16Id].blSnpVld == FALSE)
    {
        erRet = ER_DAT_EMPTY;
    }
    else
    {
        u16Cpy = (ku16Amt < kpktDtc->pktCfg[ku16Id].u16SnpSz) ?
                     ku16Amt :
                     kpktDtc->pktCfg[ku16Id].u16SnpSz;

        memcpy(kpvDat, kpktDtc->pktCfg[ku16Id].pu8Snp, u16Cpy);

        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个 DTC 的标准编号。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @param[in] ku16Id DTC 索引。
 * @param[out] kpu16Cd DTC 编号指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、配置数组或编号指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDtcCd(const stDtc* const kpktDtc, const u16 ku16Id,
              u16* const kpu16Cd)
{
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->pktCfg == NULL) || (kpu16Cd == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Id >= kpktDtc->u16Amt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        *kpu16Cd = kpktDtc->pktCfg[ku16Id].u16Cd;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 保存全部 DTC 记录。
 * @details 委托句柄中的保存回调（业务层接入 NVM/Flash）；未配置则返回不支持。
 * @param[in] kpktDtc DTC 管理器句柄。
 * @return 保存结果。
 * @retval ER_SUC 保存成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置保存回调。
 */
err erSaveDtc(const stDtc* const kpktDtc)
{
    err erRet = ER_SW_UNKN;

    if((kpktDtc == NULL) || (kpktDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(kpktDtc->pferSave == NULL)
    {
        erRet = ER_SW_NOT_SUP;
    }
    else
    {
        erRet = kpktDtc->pferSave(kpktDtc->ptRec, kpktDtc->u16Amt);
    }

    return erRet;
}

/**
 * @brief 恢复全部 DTC 记录。
 * @details 委托句柄中的恢复回调（业务层接入 NVM/Flash）；未配置则返回不支持。
 * @param[in, out] kptDtc DTC 管理器句柄。
 * @return 恢复结果。
 * @retval ER_SUC 恢复成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置恢复回调。
 */
err erLoadDtc(stDtc* const kptDtc)
{
    err erRet = ER_SW_UNKN;

    if((kptDtc == NULL) || (kptDtc->ptRec == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(kptDtc->pferLoad == NULL)
    {
        erRet = ER_SW_NOT_SUP;
    }
    else
    {
        erRet = kptDtc->pferLoad(kptDtc->ptRec, kptDtc->u16Amt);
    }

    return erRet;
}

/* == 静态函数 == */
/* -- 普通函数 -- */
/**
 * @brief 清除单条 DTC 记录的历史状态。
 * @details 清 confirmed/pending/testFailedSinceLastClear 及发生次数、时间、
 *          快照与去抖计数；不改变当前实时故障状态。
 * @param[in, out] ptRec DTC 记录指针。
 */
static void vidClrRec(stDtcRec* const ptRec)
{
    ptRec->unSts.btCfm = 0u;
    ptRec->unSts.btPend = 0u;
    ptRec->unSts.btTstFldClr = 0u;
    ptRec->u8FldCyc = 0u;
    ptRec->u8PassCyc = 0u;
    ptRec->u32OcCnt = 0u;
    ptRec->u64FstTm = 0u;
    ptRec->u64LstTm = 0u;
    ptRec->blSnpVld = FALSE;
}
