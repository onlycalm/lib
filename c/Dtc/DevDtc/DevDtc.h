/**
 * @file DevDtc.h
 * @brief DTC 业务配置模块。
 * @details 定义本项目的 DTC 枚举与快照结构。各项目的差异化 DTC 在此定义，
 *          框架部分无需修改。
 * @author Calm
 * @data 2026-10-04
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef DEV_DTC_H
#define DEV_DTC_H

#include "Typ.h"
#include "Dtc.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* ===== 枚举定义 ===== */
/* 0=传感器(A) 1=执行器(B) 2=模拟量(C) 3=通信(D)，业务层可自行定义 */
/**
 * @enum enDtcCat
 * @brief DTC 类别枚举。
 * @details 对应 2 字节编码的 bit[15:14]，共 4 大类。本业务层为嵌入式
 *          （非车规）约定。
 */
typedef enum enDtcCat
{
    DTC_CAT_SNSR, //!< 传感器类。
    DTC_CAT_ACT,  //!< 执行器类。
    DTC_CAT_ANA,  //!< 模拟量类（电流/电压/温度等）。
    DTC_CAT_COMM, //!< 通信类。
} enDtcCat;

/**
 * @enum enDevDtc
 * @brief 设备 DTC 枚举。
 * @details 作为 DTC 记录/配置数组的下标使用，框架按索引 O(1) 访问。
 */
typedef enum enDevDtc
{
    // DTC_CAT_ANA.
    DEV_DTC_TEMP_HIGH, //!< 温度过高故障（0x8115）。
    DEV_DTC_VOLT_LOW,  //!< 电压过低故障（0x8562）。

    // DTC_CAT_COMM.
    DEV_DTC_COMM_LOST, //!< 通信丢失故障（0xC100）。

    DEV_DTC_AMT,       //!< DTC 数量。
} enDevDtc;

/* ===== 结构体定义 ===== */
/**
 * @struct stDevDtcSnp
 * @brief 设备 DTC 快照（冻结帧）结构体。
 * @details 定义故障发生瞬间需要冻结的现场数据，各项目按需裁剪。
 */
typedef struct stDevDtcSnp
{
    u8 u8Ch;     //!< 通道号。
    u16 u16Temp; //!< 温度（0.1℃）。
    u16 u16Volt; //!< 电压（mV）。
} stDevDtcSnp;

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 普通函数 -- */
/**
 * @brief 初始化设备 DTC 管理器。
 * @return 处理结果。
 * @retval ER_SUC 初始化成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或配置数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 数量为 0。
 */
err erInitDevDtc(void);

/**
 * @brief 上报设备 DTC 的当前检测结果。
 * @param[in] eId DTC 枚举。
 * @param[in] bAct 当前检测结果（TRUE 故障 / FALSE 正常）。
 * @return 上报结果。
 * @retval ER_SUC 上报成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erReportDevDtc(enDevDtc eId, bl bAct);

/**
 * @brief 结束设备 DTC 的当前操作循环并评估。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erEndCycDevDtc(void);

/**
 * @brief 开始设备 DTC 的一个新操作循环。
 * @return 处理结果。
 * @retval ER_SUC 处理成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erNewCycDevDtc(void);

/**
 * @brief 清除单个设备 DTC。
 * @param[in] eId DTC 枚举。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err erClrDevDtc(enDevDtc eId);

/**
 * @brief 清除全部设备 DTC。
 * @return 清除结果。
 * @retval ER_SUC 清除成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空指针。
 */
err erClrAllDevDtc(void);

/**
 * @brief 获取单个设备 DTC 的状态字节。
 * @param[in] eId DTC 枚举。
 * @param[out] kptSts 状态字节指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或状态指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcSts(enDevDtc eId, unDtcSts* const kptSts);

/**
 * @brief 获取单个设备 DTC 的发生次数。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu32Cnt 发生次数指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或次数指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcCnt(enDevDtc eId, u32* const kpu32Cnt);

/**
 * @brief 获取单个设备 DTC 的首次/最近发生时间。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu64FstTm 首次发生时间指针。
 * @param[out] kpu64LstTm 最近发生时间指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或时间指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcTm(enDevDtc eId, u64* const kpu64FstTm,
                 u64* const kpu64LstTm);

/**
 * @brief 获取单个设备 DTC 的快照。
 * @param[in] eId DTC 枚举。
 * @param[out] kptSnp 快照指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、记录数组或快照指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 * @retval ER_SW_NOT_SUP 该 DTC 未配置快照缓冲区。
 * @retval ER_DAT_EMPTY 快照尚未捕获。
 */
err eGetDevDtcSnp(enDevDtc eId, stDevDtcSnp* const kptSnp);

/**
 * @brief 获取单个设备 DTC 的标准编号。
 * @param[in] eId DTC 枚举。
 * @param[out] kpu16Cd DTC 编号指针。
 * @return 获取结果。
 * @retval ER_SUC 获取成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄、配置数组或编号指针为空。
 * @retval ER_SW_INV_PARAM DTC 索引越界。
 */
err eGetDevDtcCd(enDevDtc eId, u16* const kpu16Cd);

/**
 * @brief 保存全部设备 DTC 记录到 NVM。
 * @return 保存结果。
 * @retval ER_SUC 保存成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置保存回调。
 */
err erSaveDevDtc(void);

/**
 * @brief 从 NVM 恢复全部设备 DTC 记录。
 * @return 恢复结果。
 * @retval ER_SUC 恢复成功。
 * @retval ER_SW_UNKN 未知错误。
 * @retval ER_SW_NUL_PTR 句柄或记录数组为空。
 * @retval ER_SW_NOT_SUP 未配置恢复回调。
 */
err erLoadDevDtc(void);

#ifdef __cplusplus
}
#endif

#endif // DEV_DTC_H
