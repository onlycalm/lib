/**
 * @file Mon.c
 * @brief 模拟量监控框架实现。
 * @details 实现监控器的初始化、周期处理及故障态查询。框架不定义具体规则，
 *          通过句柄绑定的规则大表驱动，支持多实例。
 * @author Calm
 * @data 2026-10-05
 * @version v1.0.0
 * @copyright Calm
 */

#include "Mon.h"
#include "Typ.h"
#define ER_DOM     ER_DOM_LIB
#define ER_SUB_DOM ER_SUB_DOM_LIB_C
#define ER_MOD     ER_MOD_MON
#include "Er.h"

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化监控器。
 * @details 校验句柄参数与各规则通道号合法性，并清空全部运行时记录。
 * @param[in, out] kptMon 监控器句柄。
 * @return 初始化结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 * @retval ER_SW_INV_PARAM 规则数量为 0 或存在通道号越界。
 */
err erInitMon(stMon* const kptMon)
{
    u16 u16Idx = 0u;
    err erRet = ER_SW_UNKN;

    if((kptMon == NULL) || (kptMon->pktRule == NULL) ||
       (kptMon->ptRec == NULL) || (kptMon->ps32Dat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(kptMon->u16RuleAmt == 0u)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        erRet = ER_SUC;

        // 校验各规则通道号是否越界。
        for(u16Idx = 0u; u16Idx < kptMon->u16RuleAmt; u16Idx++)
        {
            if(kptMon->pktRule[u16Idx].u8Ch >= kptMon->u8ChAmt)
            {
                erRet = ER_SW_INV_PARAM;
                break;
            }
        }
    }

    // 参数合法时清空全部运行时记录。
    if(erRet == ER_SUC)
    {
        for(u16Idx = 0u; u16Idx < kptMon->u16RuleAmt; u16Idx++)
        {
            kptMon->ptRec[u16Idx].u32DetCnt = 0u;
            kptMon->ptRec[u16Idx].u32FltCnt = 0u;
            kptMon->ptRec[u16Idx].u32RcvCnt = 0u;
            kptMon->ptRec[u16Idx].blFlt = FALSE;
        }
    }

    return erRet;
}

/**
 * @brief 监控器周期处理函数。
 * @details 遍历全部规则，先检查使能前提（不满足则视为无故障并清空去抖计数），
 *          到达检测点的规则再做越限判定并推进去抖计数，达到确认/恢复阈值时
 *          触发对应回调。
 * @param[in, out] kptMon 监控器句柄。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_NUL_PTR 句柄、规则表、记录数组或采集数组为空指针。
 */
err erTckMon(stMon* const kptMon)
{
    u16 u16Idx = 0u;
    const stMonRule* pktRule = NULL;
    stMonRec* ptRec = NULL;
    s32 s32Val = 0;
    bl blOver = FALSE;
    bl blNml = FALSE;
    err erRet = ER_SW_UNKN;

    if((kptMon == NULL) || (kptMon->pktRule == NULL) ||
       (kptMon->ptRec == NULL) || (kptMon->ps32Dat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else
    {
        erRet = ER_SUC;

        for(u16Idx = 0u; u16Idx < kptMon->u16RuleAmt; u16Idx++)
        {
            pktRule = &kptMon->pktRule[u16Idx];
            ptRec = &kptMon->ptRec[u16Idx];

            // 使能前提不满足：视为无故障并清空去抖计数，跳过本次检测。
            if((pktRule->pfbEn != NULL) && (pktRule->pfbEn() == FALSE))
            {
                ptRec->u32FltCnt = 0u;
                ptRec->u32RcvCnt = 0u;
                ptRec->blFlt = FALSE;
                continue;
            }

            // 检测周期计时递增，未达周期则跳过本次检测。
            ptRec->u32DetCnt++;

            if(ptRec->u32DetCnt < pktRule->u32DetPrd)
            {
                continue;
            }

            // 到达检测点：复位周期计时并采样。
            ptRec->u32DetCnt = 0u;
            s32Val = kptMon->ps32Dat[pktRule->u8Ch];

            // 按方向分类采样结果：越限侧 / 正常侧 / 死区。
            if(pktRule->eDir == MON_DIR_UP)
            {
                blOver = (s32Val > (pktRule->s32Thr + pktRule->s32Hys));
                blNml = (s32Val <= pktRule->s32Thr);
            }
            else
            {
                blOver = (s32Val < (pktRule->s32Thr - pktRule->s32Hys));
                blNml = (s32Val >= pktRule->s32Thr);
            }

            if(blOver == TRUE)
            {
                // 越限侧：推进故障确认去抖。
                ptRec->u32RcvCnt = 0u;

                if(ptRec->blFlt == FALSE)
                {
                    ptRec->u32FltCnt++;

                    if(ptRec->u32FltCnt >= pktRule->u32CfmTm)
                    {
                        ptRec->blFlt = TRUE;

                        if(pktRule->pfvidEvt != NULL)
                        {
                            pktRule->pfvidEvt(pktRule->u8Ch,
                                              pktRule->eDir,
                                              MON_EVT_FLT,
                                              s32Val);
                        }
                    }
                }
            }
            else if(blNml == TRUE)
            {
                // 正常侧：推进故障恢复去抖。
                ptRec->u32FltCnt = 0u;

                if(ptRec->blFlt == TRUE)
                {
                    ptRec->u32RcvCnt++;

                    if(ptRec->u32RcvCnt >= pktRule->u32RcvTm)
                    {
                        ptRec->blFlt = FALSE;

                        if(pktRule->pfvidEvt != NULL)
                        {
                            pktRule->pfvidEvt(pktRule->u8Ch,
                                              pktRule->eDir,
                                              MON_EVT_RCV,
                                              s32Val);
                        }
                    }
                }
            }
            else
            {
                // 死区：连续越限/正常均中断，复位去抖计数，维持当前故障态。
                ptRec->u32FltCnt = 0u;
                ptRec->u32RcvCnt = 0u;
            }
        }
    }

    return erRet;
}

/**
 * @brief 获取单条规则当前是否故障。
 * @param[in] kpktMon 监控器句柄。
 * @param[in] ku16Rule 规则下标。
 * @param[out] kpbFlt 故障态指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或故障态指针为空。
 * @retval ER_SW_INV_PARAM 规则下标越界。
 */
err eGetMonFlt(const stMon* const kpktMon, const u16 ku16Rule, bl* const kpbFlt)
{
    err erRet = ER_SW_UNKN;

    if((kpktMon == NULL) || (kpktMon->ptRec == NULL) || (kpbFlt == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku16Rule >= kpktMon->u16RuleAmt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        *kpbFlt = kpktMon->ptRec[ku16Rule].blFlt;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 设置单个通道的采集值。
 * @details 写入句柄绑定的采集数组，供 erTckMon 在到达检测点时读取判定。
 * @param[in, out] kptMon 监控器句柄。
 * @param[in] ku8Ch 通道号。
 * @param[in] ks32Val 采集值（已换算单位）。
 * @return 设置结果。
 * @retval ER_SUC 设置成功。
 * @retval ER_SW_NUL_PTR 句柄或采集数组为空指针。
 * @retval ER_SW_INV_PARAM 通道号越界。
 */
err erSetMonVal(stMon* const kptMon, const u8 ku8Ch, const s32 ks32Val)
{
    err erRet = ER_SW_UNKN;

    if((kptMon == NULL) || (kptMon->ps32Dat == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku8Ch >= kptMon->u8ChAmt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        kptMon->ps32Dat[ku8Ch] = ks32Val;
        erRet = ER_SUC;
    }

    return erRet;
}

/**
 * @brief 获取单个通道的采集值。
 * @param[in] kpktMon 监控器句柄。
 * @param[in] ku8Ch 通道号。
 * @param[out] kps32Val 采集值指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_NUL_PTR 句柄、采集数组或值指针为空。
 * @retval ER_SW_INV_PARAM 通道号越界。
 */
err eGetMonVal(const stMon* const kpktMon, const u8 ku8Ch, s32* const kps32Val)
{
    err erRet = ER_SW_UNKN;

    if((kpktMon == NULL) || (kpktMon->ps32Dat == NULL) || (kps32Val == NULL))
    {
        erRet = ER_SW_NUL_PTR;
    }
    else if(ku8Ch >= kpktMon->u8ChAmt)
    {
        erRet = ER_SW_INV_PARAM;
    }
    else
    {
        *kps32Val = kpktMon->ps32Dat[ku8Ch];
        erRet = ER_SUC;
    }

    return erRet;
}
