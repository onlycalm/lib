/**
 * @file Mon.h
 * @brief 模拟量监控框架。
 * @details 定义监控规则/记录/句柄结构体及 API。框架周期性对采集模块传入的
 *          模拟量数组做阈值越限检测，经「回差 + 时间」双重去抖后，在故障发生
 *          或恢复时触发回调。框架本身不含具体规则，具体监控哪些通道及其阈值、
 *          周期、时间、回调由业务层（如 DevMon）通过规则大表静态提供。
 *          框架为可移植模块，支持多实例：每个监控器持有独立句柄 stMon。
 * @author Calm
 * @data 2026-10-05
 * @version v1.0.0
 * @note 后续优化点：当前回调传入的是「消抖确认那一拍」的采样值；如需记录
 *       「故障刚发生那一刻」的值（供 DTC 冻结帧快照使用），可在 stMonRec 中
 *       增加首次越限锁存字段，确认后回调改传该锁存值。
 * @copyright Calm
 */

#ifndef MON_H
#define MON_H

#include "Typ.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ===== 枚举定义 ===== */
/**
 * @enum enMonDir
 * @brief 规则方向枚举。
 * @details 上限规则值偏高判故障，下限规则值偏低判故障。
 */
typedef enum
{
    MON_DIR_UP, //!< 上限规则。
    MON_DIR_DN, //!< 下限规则。

    MON_DIR_AMT, //!< 方向数量。
} enMonDir;

/**
 * @enum enMonEvt
 * @brief 故障事件枚举。
 * @details 故障确认或恢复时经回调上报的事件类型。
 */
typedef enum
{
    MON_EVT_FLT, //!< 故障发生。
    MON_EVT_RCV, //!< 故障恢复。

    MON_EVT_AMT, //!< 事件数量。
} enMonEvt;

/* ===== 结构体定义 ===== */
/**
 * @struct stMonRule
 * @brief 单条监控规则配置。
 * @details 由业务层静态提供，框架只读。一条规则对应一个通道的单一方向
 *          （上限或下限），一个通道的上限、下限各占一条规则。
 *          使能前提回调返回 TRUE 才检测，否则视为无故障并清空去抖计数。
 *          故障/恢复回调入参依次为通道号、方向、事件、当前采样值。
 */
typedef struct stMonRule
{
    u8 u8Ch;       //!< 通道号（采集数组下标）。
    enMonDir eDir; //!< 方向（上限/下限）。
    s32 s32Thr;    //!< 阈值（已换算单位）。
    s32 s32Hys;    //!< 回差（非负），0 表示无回差消抖。
    u32 u32DetPrd; //!< 检测周期（tick 数），0 表示每 tick 都检测。
    u32 u32CfmTm;  //!< 确认时间（连续越限检测次数），0 表示立即故障。
    u32 u32RcvTm;  //!< 恢复时间（连续正常检测次数），0 表示立即恢复。
    bl (*pfbEn)(void); //!< 使能前提回调（返回 TRUE 才检测，NULL 表示始终使能）。
    void (*pfvidEvt)(u8, enMonDir, enMonEvt, s32); //!< 故障/恢复回调（可为空）。
} stMonRule;

/**
 * @struct stMonRec
 * @brief 单条规则的运行时记录。
 * @details 由框架管理，保存检测周期计时、连续越限/正常去抖计数与当前故障态。
 */
typedef struct stMonRec
{
    u32 u32DetCnt; //!< 检测周期计时（自上次采样起累计 tick 数）。
    u32 u32FltCnt; //!< 连续越限检测次数。
    u32 u32RcvCnt; //!< 连续正常检测次数。
    bl blFlt;      //!< 当前故障态（TRUE 故障 / FALSE 正常）。
} stMonRec;

/**
 * @struct stMon
 * @brief 监控器句柄。
 * @details 持有规则表、记录数组、采集数组指针及规则/通道数量。业务层静态实例化
 *          并绑定后传入框架 API。
 */
typedef struct stMon
{
    const stMonRule* pktRule; //!< 规则表指针（与记录一一对应）。
    stMonRec* ptRec;          //!< 运行时记录数组。
    s32* ps32Dat;             //!< 采集数组指针（下标即通道号，tick 只读）。
    u16 u16RuleAmt;           //!< 规则数量。
    u8 u8ChAmt;               //!< 通道数量（用于越界校验）。
} stMon;

/* ===== 函数声明 ===== */
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
err erInitMon(stMon* const kptMon);

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
err erTckMon(stMon* const kptMon);

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
err eGetMonFlt(const stMon* const kpktMon, const u16 ku16Rule,
               bl* const kpbFlt);

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
err erSetMonVal(stMon* const kptMon, const u8 ku8Ch, const s32 ks32Val);

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
err eGetMonVal(const stMon* const kpktMon, const u8 ku8Ch, s32* const kps32Val);

#ifdef __cplusplus
}
#endif

#endif // MON_H
