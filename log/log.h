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
#define LOG                         ENABLE              //!< Log模块开关。
#endif //LOG

#ifndef LOG_LV
#define LOG_LV                      3u                  //!< 默认Log等级。
#endif //LOG_LV

//=============================================================================
//Log等级
#define FATAL                       5u                  //!< 致命。
#define ERROR                       4u                  //!< 错误。
#define WARN                        3u                  //!< 警告。
#define DEBUG                       2u                  //!< 调试。
#define TRACE                       1u                  //!< 跟踪。
#define INFO                        0u                  //!< 信息。

//-----------------------------------------------------------------------------
//宏函数
#if LOG == ENABLE
#define ToStr(x)                    Str(x)              //!< 宏值转字符串。
#define Str(x)                      #x                  //!< 名转字符串。
#define FmtLog(Lv)                  "[" Str(Lv) "] - [" __FILE__ " <" ToStr(__LINE__) ">](" __FUNCTION__ "): "

#if LOG_LV >= FATAL
#define PrtLogFatal(Str, ...)       printf(FmtLog(FATAL) Str "\n", ##__VA_ARGS__)
#else
#define PrtLogFatal()               ((void)0u)
#endif //LOG_LV

#if LOG_LV >= ERROR
#define PrtLogError(Str, ...)       printf(FmtLog(ERROR) Str "\n", ##__VA_ARGS__)
#else
#define PrtLogError()               ((void)0u)
#endif //LOG_LV

#if LOG_LV >= WARN
#define PrtLogWarn(Str, ...)        printf(FmtLog(WARN) Str "\n", ##__VA_ARGS__)
#else
#define PrtLogWarn()                ((void)0u)
#endif //LOG_LV

#if LOG_LV >= DEBUG
#define PrtLogDebug(Str, ...)       printf(FmtLog(DEBUG) Str "\n", ##__VA_ARGS__)
#else
#define PrtLogDebug()               ((void)0u)
#endif //LOG_LV

#if LOG_LV >= TRACE
#define PrtLogTrace(Str, ...)       printf(FmtLog(TRACE) Str "\n", ##__VA_ARGS__)
#define PrtLogEntFun()              printf(FmtLog(TRACE) "<-----Enter function----->\n")
#define PrtLogExFun()               printf(FmtLog(TRACE) "<-----Exit function----->\n")
#else
#define PrtLogTrace()               ((void)0u)
#define PrtLogEntFun()              ((void)0u)
#define PrtLogExFun()               ((void)0u)
#endif //LOG_LV

#if LOG_LV >= INFO
#define PrtLogInfo(Str, ...)        printf(FmtLog(INFO) Str "\n", ##__VA_ARGS__)
#define PrtLogRtn(Rtn)              printf(FmtLog(INFO) "%s: %d\n", #Rtn, Rtn)
#define PrtLogVar(Var)              printf(FmtLog(INFO) "%s: %d\n", #Var, Var)
#else
#define PrtLogInfo()                ((void)0u)
#define PrtLogRtn()                 ((void)0u)
#define PrtLogVar()                 ((void)0u)
#endif //LOG_LV

#else
#define FmtLog()                    ((void)0u)
#define PrtLogFatal()               ((void)0u)
#define PrtLogError()               ((void)0u)
#define PrtLogWarn()                ((void)0u)
#define PrtLogDebug()               ((void)0u)
#define PrtLogTrace()               ((void)0u)
#define PrtLogEntFun()              ((void)0u)
#define PrtLogExFun()               ((void)0u)
#define PrtLogInfo()                ((void)0u)
#define PrtLogRtn()                 ((void)0u)
#define PrtLogVar()                 ((void)0u)
#endif //LOG

#endif //LOG_H
