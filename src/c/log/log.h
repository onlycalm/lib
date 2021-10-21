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
#define LOG                      ENABLE              //!<Log模块开关。
#endif //LOG

#ifndef LOG_LV
#define LOG_LV                   TRACE               //!<默认Log等级。
#endif //LOG_LV

//=============================================================================
//Log等级
#define LOG_CRITICAL             50u                 //!<致命。
#define LOG_ERROR                40u                 //!<错误。
#define LOG_WARNING              30u                 //!<警告。
#define LOG_SUCCESS              25u                 //!<成功。
#define LOG_INFO                 20u                 //!<信息。
#define LOG_DEBUG                10u                 //!<调试。
#define LOG_TRACE                5u                  //!<跟踪。

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#define FmtLog(Lv, Str)          "[" Lv "] %s:%s:%d - " Str "\n", GetFileNm(__FILE__), __FUNCTION__, __LINE__

#if LOG_LV <= LOG_CRITICAL
#define LogCrt(Str, ...)         printf(FmtLog("CRITICAL", Str), ##__VA_ARGS__)
#else
#define LogCrt()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_ERROR
#define LogErr(Str, ...)         printf(FmtLog("ERROR", Str) , ##__VA_ARGS__)
#else
#define LogErr()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_WARNING
#define LogWrn(Str, ...)         printf(FmtLog("WARNING", Str), ##__VA_ARGS__)
#else
#define LogWrn()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_SUCCESS
#define LogSuc(Str, ...)         printf(FmtLog("SUCCESS", Str), ##__VA_ARGS__)
#else
#define LogSuc()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_INFO
#define LogInf(Str, ...)         printf(FmtLog("INFO", Str), ##__VA_ARGS__)
#else
#define LogInf ()                ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_DEBUG
#define LogDbg(Str, ...)         printf(FmtLog("DEBUG", Str), ##__VA_ARGS__)
#else
#define LogDbg()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV <= LOG_TRACE
#define LogTr(Str, ...)          printf(FmtLog("TRACE", Str), ##__VA_ARGS__)
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
