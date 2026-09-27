/**
 * @file Log.h
 * @brief log模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "Com.h"

/**
 * @defgroup LogModule 日志模块
 * @brief 分级彩色日志输出模块，支持终端与日志文件双输出。
 * @details
 * 提供七个日志等级（CRITICAL / ERROR / WARNING / SUCCESS / INFO / DEBUG / TRACE），
 * 支持等级过滤、时间戳、字体样式与颜色、背景颜色等可配置项，
 * 并可自动创建日志目录、同时输出到终端与日志文件。
 */

/** @addtogroup LogModule
 * @{
 */

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//配置宏
#ifndef LOG
#define LOG ENABLE //!< Log模块开关。
#endif // LOG

#ifndef LOG_LV
#define LOG_LV LOG_TRACE //!<默认Log等级。
#endif // LOG_LV

#ifndef LOG_FILE
#define LOG_FILE ENABLE //!< 日志文件输出开关。
#endif // LOG_FILE

#ifndef LOG_TM_STMP
#define LOG_TM_STMP ENABLE //!< 日志时间戳开关。
#endif // LOG_TM_STMP

#ifndef LOG_STY
#define LOG_STY ENABLE //!< 日志样式开关。
#endif // LOG_STY

#ifndef LOG_FNT
#define LOG_FNT ENABLE //!< 字体样式开关。
#endif // LOG_FNT

#ifndef LOG_FNT_CLR
#define LOG_FNT_CLR ENABLE //!< 字体颜色开关。
#endif // LOG_FNT_CLR

#ifndef LOG_BG_CLR
#define LOG_BG_CLR ENABLE //!< 背景颜色开关。
#endif // LOG_BG_CLR

//=============================================================================
//Log等级
#define LOG_CRITICAL             50u                 //!<致命。
#define LOG_ERROR                40u                 //!<错误。
#define LOG_WARNING              30u                 //!<警告。
#define LOG_SUCCESS              25u                 //!<成功。
#define LOG_INFO                 20u                 //!<信息。
#define LOG_DEBUG                10u                 //!<调试。
#define LOG_TRACE                5u                  //!<跟踪。

//=============================================================================
#define LOG_STY_RESET       "\033[0m"

// 字体样式。
#define LOG_FNT_DFLT        0
#define LOG_FNT_BOLD        1
#define LOG_FNT_UNDERLINE   4
#define LOG_FNT_BLINK       5
#define LOG_FNT_REVERSE     7
#define LOG_FNT_HIDDEN      8

// 前景色（字体颜色）。
#define LOG_FNT_CLR_DFLT    39
#define LOG_FNT_CLR_BLACK   30
#define LOG_FNT_CLR_GREY    90
#define LOG_FNT_CLR_RED     91
#define LOG_FNT_CLR_GREEN   92
#define LOG_FNT_CLR_YELLOW  93
#define LOG_FNT_CLR_BLUE    94
#define LOG_FNT_CLR_MAGENTA 95
#define LOG_FNT_CLR_CYAN    96
#define LOG_FNT_CLR_WHITE   37

// 背景色。
#define LOG_BG_CLR_DFLT    49
#define LOG_BG_CLR_BLACK   40
#define LOG_BG_CLR_GREY    100
#define LOG_BG_CLR_RED     101
#define LOG_BG_CLR_GREEN   102
#define LOG_BG_CLR_YELLOW  103
#define LOG_BG_CLR_BLUE    104
#define LOG_BG_CLR_MAGENTA 105
#define LOG_BG_CLR_CYAN    106
#define LOG_BG_CLR_WHITE   47

#if LOG_FNT == ENABLE
#define LOG_FNT_CRITICAL LOG_FNT_DFLT
#define LOG_FNT_ERROR    LOG_FNT_DFLT
#define LOG_FNT_WARNING  LOG_FNT_DFLT
#define LOG_FNT_SUCCESS  LOG_FNT_DFLT
#define LOG_FNT_INFO     LOG_FNT_DFLT
#define LOG_FNT_DEBUG    LOG_FNT_DFLT
#define LOG_FNT_TRACE    LOG_FNT_DFLT
#else
#define LOG_FNT_CRITICAL LOG_FNT_DFLT
#define LOG_FNT_ERROR    LOG_FNT_DFLT
#define LOG_FNT_WARNING  LOG_FNT_DFLT
#define LOG_FNT_SUCCESS  LOG_FNT_DFLT
#define LOG_FNT_INFO     LOG_FNT_DFLT
#define LOG_FNT_DEBUG    LOG_FNT_DFLT
#define LOG_FNT_TRACE    LOG_FNT_DFLT
#endif // LOG_FNT == ENABLE

#if LOG_FNT_CLR == ENABLE
#define LOG_FNT_CLR_CRITICAL LOG_FNT_CLR_MAGENTA
#define LOG_FNT_CLR_ERROR    LOG_FNT_CLR_RED
#define LOG_FNT_CLR_WARNING  LOG_FNT_CLR_YELLOW
#define LOG_FNT_CLR_SUCCESS  LOG_FNT_CLR_GREEN
#define LOG_FNT_CLR_INFO     LOG_FNT_CLR_WHITE
#define LOG_FNT_CLR_DEBUG    LOG_FNT_CLR_BLUE
#define LOG_FNT_CLR_TRACE    LOG_FNT_CLR_GREY
#else
#define LOG_FNT_CLR_CRITICAL LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_ERROR    LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_WARNING  LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_SUCCESS  LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_INFO     LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_DEBUG    LOG_FNT_CLR_DFLT
#define LOG_FNT_CLR_TRACE    LOG_FNT_CLR_DFLT
#endif // LOG_FNT_CLR == ENABLE

#if LOG_BG_CLR == ENABLE
#define LOG_BG_CLR_CRITICAL LOG_BG_CLR_DFLT
#define LOG_BG_CLR_ERROR    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_WARNING  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_SUCCESS  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_INFO     LOG_BG_CLR_DFLT
#define LOG_BG_CLR_DEBUG    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_TRACE    LOG_BG_CLR_DFLT
#else
#define LOG_BG_CLR_CRITICAL LOG_BG_CLR_DFLT
#define LOG_BG_CLR_ERROR    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_WARNING  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_SUCCESS  LOG_BG_CLR_DFLT
#define LOG_BG_CLR_INFO     LOG_BG_CLR_DFLT
#define LOG_BG_CLR_DEBUG    LOG_BG_CLR_DFLT
#define LOG_BG_CLR_TRACE    LOG_BG_CLR_DFLT
#endif // LOG_BG_CLR == ENABLE

#if LOG_STY == ENABLE
#define LOG_STY_CRITICAL "\033[" pcToStr(LOG_FNT_CRITICAL) ";"\
                                 pcToStr(LOG_FNT_CLR_CRITICAL)  ";"\
                                 pcToStr(LOG_BG_CLR_CRITICAL) "m"
#define LOG_STY_ERROR "\033[" pcToStr(LOG_FNT_ERROR) ";"\
                              pcToStr(LOG_FNT_CLR_ERROR)  ";"\
                              pcToStr(LOG_BG_CLR_ERROR) "m"
#define LOG_STY_WARNING "\033[" pcToStr(LOG_FNT_WARNING) ";"\
                                pcToStr(LOG_FNT_CLR_WARNING)  ";"\
                                pcToStr(LOG_BG_CLR_WARNING) "m"
#define LOG_STY_SUCCESS "\033[" pcToStr(LOG_FNT_SUCCESS) ";"\
                                pcToStr(LOG_FNT_CLR_SUCCESS)  ";"\
                                pcToStr(LOG_BG_CLR_SUCCESS) "m"
#define LOG_STY_INFO "\033[" pcToStr(LOG_FNT_INFO) ";"\
                             pcToStr(LOG_FNT_CLR_INFO)  ";"\
                             pcToStr(LOG_BG_CLR_INFO) "m"
#define LOG_STY_DEBUG "\033[" pcToStr(LOG_FNT_DEBUG) ";"\
                              pcToStr(LOG_FNT_CLR_DEBUG)  ";"\
                              pcToStr(LOG_BG_CLR_DEBUG) "m"
#define LOG_STY_TRACE "\033[" pcToStr(LOG_FNT_TRACE) ";"\
                              pcToStr(LOG_FNT_CLR_TRACE)  ";"\
                              pcToStr(LOG_BG_CLR_TRACE) "m"
#else
#define LOG_STY_CRITICAL
#define LOG_STY_ERROR
#define LOG_STY_WARNING
#define LOG_STY_SUCCESS
#define LOG_STY_INFO
#define LOG_STY_DEBUG
#define LOG_STY_TRACE
#endif // LOG_STY == ENABLE

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
/**
 * @def FmtLog
 * @brief 生成带等级、时间戳、文件、函数、行号的日志前缀。
 * @param[in] Lv 日志等级字符串。
 * @param[in] Str 格式化字符串。
 * @note 供各 Log 等级宏内部使用。
 */
#if LOG_TM_STMP == ENABLE
#define FmtLog(Lv, Str) "%s" "[" Lv "] %s:%s:%d - " Str LOG_STY_RESET "\n",\
                        pcGetTmStmp(), pcGetFileNm(__FILE__), __FUNCTION__, __LINE__
#else
#define FmtLog(Lv, Str) "[" Lv "] %s:%s:%d - " Str LOG_STY_RESET "\n",\
                        pcGetFileNm(__FILE__), __FUNCTION__, __LINE__
#endif // ENABLE

/**
 * @def vidLog
 * @brief 输出日志到标准输出与日志文件。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 根据 LOG_FILE 开关决定是否同时写入日志文件。
 */
#if LOG_FILE == ENABLE
#define vidLog(Str, ...) \
    do { \
        printf(Str, ##__VA_ARGS__); \
        vidLogPrintf(Str, ##__VA_ARGS__); \
    } while (0u)
#else
#define vidLog(Str, ...) \
    do { \
        printf(Str, ##__VA_ARGS__); \
    } while (0u)
#endif // LOG_FILE

/**
 * @def LogCrt
 * @brief 输出 CRITICAL（致命）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 CRITICAL 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_CRITICAL
#define LogCrt(Str, ...) vidLog(LOG_STY_CRITICAL FmtLog("CRITICAL", Str), ##__VA_ARGS__)
#else
#define LogCrt(Str, ...) ((void)0u)
#endif // LOG_LV

/**
 * @def LogErr
 * @brief 输出 ERROR（错误）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 ERROR 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_ERROR
#define LogErr(Str, ...) vidLog(LOG_STY_ERROR FmtLog("ERROR", Str) , ##__VA_ARGS__)
#else
#define LogErr(Str, ...) ((void)0u)
#endif //LOG_LV

/**
 * @def LogWrn
 * @brief 输出 WARNING（警告）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 WARNING 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_WARNING
#define LogWrn(Str, ...) vidLog(LOG_STY_WARNING FmtLog("WARNING", Str), ##__VA_ARGS__)
#else
#define LogWrn(Str, ...) ((void)0u)
#endif //LOG_LV

/**
 * @def LogScs
 * @brief 输出 SUCCESS（成功）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 SUCCESS 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_SUCCESS
#define LogScs(Str, ...) vidLog(LOG_STY_SUCCESS FmtLog("SUCCESS", Str), ##__VA_ARGS__)
#else
#define LogScs(Str, ...) ((void)0u)
#endif //LOG_LV

/**
 * @def LogInf
 * @brief 输出 INFO（信息）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 INFO 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_INFO
#define LogInf(Str, ...) vidLog(LOG_STY_INFO FmtLog("INFO", Str), ##__VA_ARGS__)
#else
#define LogInf(Str, ...) ((void)0u)
#endif //LOG_LV

/**
 * @def LogDbg
 * @brief 输出 DEBUG（调试）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 DEBUG 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_DEBUG
#define LogDbg(Str, ...) vidLog(LOG_STY_DEBUG FmtLog("DEBUG", Str), ##__VA_ARGS__)
#else
#define LogDbg(Str, ...) ((void)0u)
#endif //LOG_LV

/**
 * @def LogTr
 * @brief 输出 TRACE（跟踪）等级日志。
 * @param[in] Str 格式化字符串。
 * @param[in] ... 可变参数列表。
 * @note 当 LOG_LV 高于 TRACE 等级时，该宏展开为空操作。
 */
#if LOG_LV <= LOG_TRACE
#define LogTr(Str, ...) vidLog(LOG_STY_TRACE FmtLog("TRACE", Str), ##__VA_ARGS__)
#else
#define LogTr(Str, ...) ((void)0u)
#endif //LOG_LV
#else
#define FmtLog(Lv, Str)  ((void)0u)
#define LogCrt(Str, ...) ((void)0u)
#define LogErr(Str, ...) ((void)0u)
#define LogWrn(Str, ...) ((void)0u)
#define LogScs(Str, ...) ((void)0u)
#define LogInf(Str, ...) ((void)0u)
#define LogDbg(Str, ...) ((void)0u)
#define LogTr(Str, ...)  ((void)0u)
#endif //LOG

extern const char* pcGetTmStmp(void);
extern char* pcGetFileNm(const char* const cpPath);
extern err erInitLog(const char* pcFilePth);
extern void vidClsLog(void);
extern void vidLogPrintf(const char* pcFmt, ...);

#ifdef __cplusplus
}
#endif // __cplusplus

/** @} */

#endif //LOG_H
