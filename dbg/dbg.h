/**
 * @file dbg.h
 * @brief Debug模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef DBG_H
#define DBG_H

#include "header.h"

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//配置宏
#ifndef DBG
#define DBG                         ENABLE              //!< DBG模块开关。
#endif //DBG

//=============================================================================
//Log等级
#define LOG_LV_FATAL                FATAL               //!< 致命。
#define LOG_LV_ERROR                ERROR               //!< 错误。
#define LOG_LV_WARN                 WARN                //!< 警告。
#define LOG_LV_INFO                 INFO                //!< 信息。
#define LOG_LV_DEBUG                DEBUG               //!< 调试。
#define LOG_LV_TRACE                TRACE               //!< 跟踪。

//-----------------------------------------------------------------------------
//宏函数
#if DBG == ENABLE
#define ToStr(x)                    Str(x)              //!< 宏值转字符串。
#define Str(x)                      #x                  //!< 转字符串。
#define FmtLog(Lv)                  "[" ToStr(Lv) "] - [" __FILE__ " <" ToStr(__LINE__) ">](" __FUNCTION__ "): "
#define PrtLogFatal(Str, ...)       printf(FmtLog(LOG_LV_FATAL) Str "\n", ##__VA_ARGS__)
#define PrtLogError(Str, ...)       printf(FmtLog(LOG_LV_ERROR) Str "\n", ##__VA_ARGS__)
#define PrtLogWarn(Str, ...)        printf(FmtLog(LOG_LV_WARN) Str "\n", ##__VA_ARGS__)
#define PrtLogInfo(Str, ...)        printf(FmtLog(LOG_LV_INFO) Str "\n", ##__VA_ARGS__)
#define PrtLogDebug(Str, ...)       printf(FmtLog(LOG_LV_DEBUG) Str "\n", ##__VA_ARGS__)
#define PrtLogTrace(Str, ...)       printf(FmtLog(LOG_LV_TRACE) Str "\n", ##__VA_ARGS__)
#define PrtLogEntFun()              printf(FmtLog(LOG_LV_TRACE) "<-----Enter function----->\n")
#define PrtLogExFun()               printf(FmtLog(LOG_LV_TRACE) "<-----Exit function----->\n")
#define PrtLogRtn(Rtn)              printf(FmtLog(LOG_LV_INFO) "%s: %d\n", #Rtn, Rtn)
#define PrtLogVar(Var)              printf(FmtLog(LOG_LV_INFO) "%s: %d\n", #Var, Var)
#elif DBG == DISABLE
#define FmtLog()                    ((void)0u)
#define PrtLogFatal()               ((void)0u)
#define PrtLogError()               ((void)0u)
#define PrtLogWarn()                ((void)0u)
#define PrtLogInfo()                ((void)0u)
#define PrtLogDebug()               ((void)0u)
#define PrtLogTrace()               ((void)0u)
#define PrtLogEntFun()              ((void)0u)
#define PrtLogExFun()               ((void)0u)
#define PrtLogRtn()                 ((void)0u)
#define PrtLogVar()                 ((void)0u)
#endif //DBG

#endif //DBG_H
