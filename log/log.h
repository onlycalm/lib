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
#define LOG_LV                   3u                  //!< 默认Log等级。
#endif //LOG_LV

//=============================================================================
//Log等级
#define FATAL                    5u                  //!< 致命。
#define ERROR                    4u                  //!< 错误。
#define WARN                     3u                  //!< 警告。
#define DEBUG                    2u                  //!< 调试。
#define TRACE                    1u                  //!< 跟踪。
#define INFO                     0u                  //!< 信息。

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#define ToStr(x)                 Str(x)              //!< 宏值转字符串。
#define Str(x)                   #x                  //!< 名转字符串。
#define FmtLog(Lv)               "[" Str(Lv) "] " __FILE__ ":" __FUNCTION__ ":" ToStr(__LINE__) " - "

#if LOG_LV >= FATAL
#define LogFat(Str, ...)         printf(FmtLog(FATAL) Str "\n", ##__VA_ARGS__)
#else
#define LogFat()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= ERROR
#define LogErr(Str, ...)         printf(FmtLog(ERROR) Str "\n", ##__VA_ARGS__)
#else
#define LogErr()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= WARN
#define LogWrn(Str, ...)         printf(FmtLog(WARN) Str "\n", ##__VA_ARGS__)
#else
#define LogWrn()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= DEBUG
#define LogDbg(Str, ...)         printf(FmtLog(DEBUG) Str "\n", ##__VA_ARGS__)
#else
#define LogDbg()                 ((void)0u)
#endif //LOG_LV

#if LOG_LV >= TRACE
#define LogTr(Str, ...)          printf(FmtLog(TRACE) Str "\n", ##__VA_ARGS__)
#define LogEntFun()              printf(FmtLog(TRACE) "<-----Enter function----->\n")
#define LogExFun()               printf(FmtLog(TRACE) "<-----Exit function----->\n")
#else
#define LogTr()                  ((void)0u)
#define LogEntFun()              ((void)0u)
#define LogExFun()               ((void)0u)
#endif //LOG_LV

#if LOG_LV >= INFO
#define LogInfo(Str, ...)        printf(FmtLog(INFO) Str "\n", ##__VA_ARGS__)
#define LogRtn(Rtn)              printf(FmtLog(INFO) "%s: %d\n", #Rtn, Rtn)
#define LogVar(Var)              printf(FmtLog(INFO) "%s: %d\n", #Var, Var)
#else
#define LogInfo()                ((void)0u)
#define LogRtn()                 ((void)0u)
#define LogVar()                 ((void)0u)
#endif //LOG_LV

#else
#define FmtLog()                 ((void)0u)
#define LogFat()                 ((void)0u)
#define LogErr()                 ((void)0u)
#define LogWrn()                 ((void)0u)
#define LogDbg()                 ((void)0u)
#define LogTr()                  ((void)0u)
#define LogEntFun()              ((void)0u)
#define LogExFun()               ((void)0u)
#define LogInfo()                ((void)0u)
#define LogRtn()                 ((void)0u)
#define LogVar()                 ((void)0u)
#endif //LOG

#endif //LOG_H
