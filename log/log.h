/**
 * @file log.h
 * @brief log模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef LOG_H
#define LOG_H

#include "hdr.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//配置宏
#ifndef LOG
#define LOG                      ENABLE              //!< Log模块开关。
#endif //LOG

#ifndef LOG_LV
#define LOG_LV                   TRACE               //!< 默认Log等级。
#endif //LOG_LV

//=============================================================================
//Log等级
#define CRITICAL                 50u                 //!< 致命。
#define ERROR                    40u                 //!< 错误。
#define WARNING                  30u                 //!< 警告。
#define SUCCESS                  25u                 //!< 成功。
#define INFO                     20u                 //!< 信息。
#define DEBUG                    10u                 //!< 调试。
#define TRACE                    5u                  //!< 跟踪。

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#define ToStr(x)                 Str(x)              //!< 宏值转字符串。
#define Str(x)                   #x                  //!< 名转字符串。
#define FmtLog(Lv)               "[" Str(Lv) "] " __FILE__ ":" __FUNCTION__ ":" ToStr(__LINE__) " - "

#if LOG_LV >= CRITICAL
#define LogFat(Str, ...)         printf(FmtLog(CRITICAL) Str "\n", ##__VA_ARGS__)
#else
#define LogFat()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= ERROR
#define LogErr(Str, ...)         printf(FmtLog(ERROR) Str "\n", ##__VA_ARGS__)
#else
#define LogErr()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= WARNING
#define LogWrn(Str, ...)         printf(FmtLog(WARNING) Str "\n", ##__VA_ARGS__)
#else
#define LogWrn()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= SUCCESS
#define LogSuc(Str, ...)         printf(FmtLog(SUCCESS) Str "\n", ##__VA_ARGS__)
#else
#define LogSuc()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= INFO
#define LogInf(Str, ...)         printf(FmtLog(INFO) Str "\n", ##__VA_ARGS__)
#else
#define LogInf ()                ((void)0u)
#endif //LOG_LV

#if LOG_LV >= DEBUG
#define LogDbg(Str, ...)         printf(FmtLog(DEBUG) Str "\n", ##__VA_ARGS__)
#else
#define LogDbg()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= TRACE
#define LogTr(Str, ...)          printf(FmtLog(TRACE) Str "\n", ##__VA_ARGS__)
#else
#define LogTr()                  ((void)0u)
#endif //LOG_LV

#else
#define FmtLog()                 ((void)0u)
#define LogCrt()                 ((void)0u)
#define LogErr()                 ((void)0u)
#define LogWrn()                 ((void)0u)
#define LogScs()                 ((void)0u)
#define LogInf()                 ((void)0u)
#define LogDbg()                 ((void)0u)
#define LogTr()                  ((void)0u)
#endif //LOG

#endif //LOG_H
