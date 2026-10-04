/**
 * @file Dtc.h
 * @brief DTC 诊断故障码框架。
 * @details 定义 DTC 编号格式、状态字节、配置/记录/句柄结构体及 API。
 *          状态字节、成熟/治愈机制、发生次数、时间与快照参考汽车行业
 *          UDS/ISO 14229 的 DTC 行为；DTC 编号采用中性化类别（2 字节编码），
 *          不绑定汽车 P/C/B/U 语义，类别位具体含义由业务层定义。
 *          框架本身不定义具体 DTC，具体 DTC 编号与快照由业务层提供。
 *          框架为可移植模块，支持多实例：每个 DTC 管理器持有独立句柄 stDtc。
 * @author Calm
 * @data 2026-10-04
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef DTC_H
#define DTC_H

#include "Typ.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ===== 宏定义 ===== */
/* DTC 编号位域掩码（2 字节编码）。 */
#define DTC_CAT_BT_FLD 0x03u //!< 类别位域：bit[15:14]。
#define DTC_RSV_BT_FLD 0x01u //!< 预留位域：bit[13]。
#define DTC_SUB_BT_FLD 0x1Fu //!< 子系统位域：bit[12:8]。
#define DTC_IDX_BT_FLD 0xFFu //!< 故障序号位域：bit[7:0]。

/**
 * @def u16PkgDtc
 * @brief 构造 DTC 编号。
 * @details 将类别、预留位、子系统与故障序号打包为 2 字节编码。
 * @param[in] eCat 类别（0~3，由业务层定义）。
 * @param[in] bRsv 预留位（通常为 0）。
 * @param[in] u8Sub 子系统编号，范围 0~31。
 * @param[in] u8Idx 故障序号，范围 0~255。
 * @return 打包后的 DTC 编号。
 */
#define u16PkgDtc(eCat, bRsv, u8Sub, u8Idx) \
    ((((u16)(eCat) & (u16)DTC_CAT_BT_FLD) << 14u) | \
     (((u16)(bRsv) & (u16)DTC_RSV_BT_FLD) << 13u) | \
     (((u16)(u8Sub) & (u16)DTC_SUB_BT_FLD) << 8u) | \
     (((u16)(u8Idx) & (u16)DTC_IDX_BT_FLD) << 0u))

/* ===== 联合体定义 ===== */
/**
 * @union unDtcSts
 * @brief DTC 状态字节。
 * @details 与 UDS/ISO 14229-1 附录 D 的 DTC 状态字节位定义一致。
 *          bit4（自上次清除后测试未完成）与 bit6（本操作循环测试未完成）
 *          因缺少「测试完成」上报信号，本框架不主动管理；bit7（警告灯请求）
 *          预留，均默认保持 0。
 */
typedef union unDtcSts
{
    u8 u8Dat; //!< 状态字节整体值。

    struct
    {
        u8 btTstFld      : 1u; //!< testFailed，当前测试失败（当前故障）。
        u8 btTstFldCyc   : 1u; //!< testFailedThisOperationCycle，本操作循环失败。
        u8 btPend        : 1u; //!< pendingDTC，待定。
        u8 btCfm         : 1u; //!< confirmedDTC，已确认（历史故障）。
        u8 btTstNtCplClr : 1u; //!< testNotCompletedSinceLastClear（预留）。
        u8 btTstFldClr   : 1u; //!< testFailedSinceLastClear，自上次清除后失败过。
        u8 btTstNtCplCyc : 1u; //!< testNotCompletedThisOperationCycle（预留）。
        u8 btMilReq      : 1u; //!< warningIndicatorRequested，请求点亮警告灯（预留）。
    };
} unDtcSts;

/* ===== 结构体定义 ===== */
/**
 * @struct stDtcCfg
 * @brief 单个 DTC 的配置结构体。
 * @details 由业务层静态提供，框架只读。记录 DTC 编号与快照缓冲区信息，
 *          运行时状态与配置分离，初始化不会覆盖配置。
 */
typedef struct stDtcCfg
{
    u16 u16Cd;    //!< DTC 标准编号（2 字节编码）。
    u8* pu8Snp;   //!< 快照缓冲区指针（业务层提供，可空表示无快照）。
    u16 u16SnpSz; //!< 快照缓冲区大小（字节）。
} stDtcCfg;

/**
 * @struct stDtcRec
 * @brief 单个 DTC 的运行时记录结构体。
 * @details 由框架管理，保存状态字节、成熟/治愈去抖计数、发生次数、时间与
 *          快照有效标志。初始化时全部清零。
 */
typedef struct stDtcRec
{
    unDtcSts unSts; //!< UDS 状态字节。
    u8 u8FldCyc;    //!< 连续失败操作循环计数（用于确认去抖）。
    u8 u8PassCyc;   //!< 连续正常操作循环计数（用于治愈去抖）。
    u32 u32OcCnt;   //!< 发生次数（自上次清除以来，饱和计数）。
    u64 u64FstTm;   //!< 首次发生时间。
    u64 u64LstTm;   //!< 最近发生时间。
    bl blSnpVld;    //!< 快照有效标志。
} stDtcRec;

/**
 * @struct stDtc
 * @brief DTC 管理器句柄结构体。
 * @details 持有记录数组、配置数组、DTC 数量、确认/治愈阈值及时间、快照、
 *          保存、恢复回调。业务层静态实例化并绑定后传入框架 API。
 */
typedef struct stDtc
{
    stDtcRec* ptRec;        //!< 运行时记录数组。
    const stDtcCfg* pktCfg; //!< 配置数组（与记录一一对应）。
    u16 u16Amt;             //!< DTC 数量。
    u8 u8CfmCyc;            //!< 确认阈值：连续失败循环数（典型 2）。
    u8 u8HealCyc;           //!< 治愈阈值：连续正常循环数（典型 2）。
    u64 (*pfu64GetTm)(void);                     //!< 时间源回调（可为空）。
    void (*pfvidSnp)(u16 u16Id, u8* pu8Snp);     //!< 快照捕获回调（可为空）。
    err (*pferSave)(const stDtcRec* kptRec, u16 ku16Amt); //!< 保存回调（预留）。
    err (*pferLoad)(stDtcRec* ptRec, u16 u16Amt);         //!< 恢复回调（预留）。
} stDtc;

/* ===== 函数声明 ===== */
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
err erInitDtc(stDtc* const kptDtc);

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
err erReportDtc(stDtc* const kptDtc, const u16 ku16Id, const bl kbAct);

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
err erEndCycDtc(stDtc* const kptDtc);

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
err erNewCycDtc(stDtc* const kptDtc);

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
err erClrDtc(stDtc* const kptDtc, const u16 ku16Id);

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
err erClrAllDtc(stDtc* const kptDtc);

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
               unDtcSts* const kptSts);

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
               u32* const kpu32Cnt);

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
              u64* const kpu64FstTm, u64* const kpu64LstTm);

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
               void* const kpvDat, const u16 ku16Amt);

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
              u16* const kpu16Cd);

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
err erSaveDtc(const stDtc* const kpktDtc);

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
err erLoadDtc(stDtc* const kptDtc);

#ifdef __cplusplus
}
#endif

#endif // DTC_H
