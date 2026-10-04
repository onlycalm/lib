/**
 * @file Log.c
 * @brief log模块。
 * @details 无
 * @author Calm
 * @date 2021-08-11
 * @version v1.0.0
 * @copyright Calm
 */

#include <asm-generic/errno-base.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>  // for mkdir on Unix-like systems
#include "Com.h"
#include "Log.h"
#include "Typ.h"
#define ER_DOM      ER_DOM_ORD_BOT
#define ER_SUB_DOM  ER_SUB_DOM_ORDMCU
#define ER_MOD      ER_MOD_LOG
#include "Er.h"

#ifdef LOG_H

/** @addtogroup LogModule
 * @{
 */

#define TIME_BUFFER_SIZE 31  //!< 时间戳字符串缓冲区大小（含结尾 '\0'）。
#define PTH_MAX_SIZE     256u //!< 路径最大长度。

FILE* pfLogFile = NULL; //!< 日志文件指针。

/**
 * @brief 获取当前时间戳字符串。
 * @details 获取本地时间并格式化为 "YYYY-MM-DD HH:MM:SS.uuuuuu" 形式（uuuuuu 为微秒），供日志前缀使用。
 * @return 时间戳字符串指针。
 * @retval 非 NULL 时间戳字符串。
 * @retval NULL 获取系统时间失败。
 * @see 扇入：FmtLog 宏。
 * @see 扇出：clock_gettime、localtime、strftime、snprintf、strlen。
 * @note 返回的缓冲区为静态数组，非线程安全，且会被下一次调用覆盖。
 */
const char* pcGetTmStmp(void)
{
    static char acTmStr[TIME_BUFFER_SIZE]; // 使用静态数组
    struct timespec tTmSpec = {0};
    struct tm* ptTmInfo = {0};
    char* pcTmStr = NULL;

    if(clock_gettime(CLOCK_REALTIME, &tTmSpec) != -1)
    {
        ptTmInfo = localtime(&tTmSpec.tv_sec);
        strftime(acTmStr, sizeof(acTmStr), "%Y-%m-%d %H:%M:%S", ptTmInfo);
        snprintf(acTmStr + strlen(acTmStr),
                 sizeof(acTmStr) - strlen(acTmStr),
                 ".%06ld ", (long)(tTmSpec.tv_nsec / 1000));
        pcTmStr = acTmStr;
    }
    else
    {
        pcTmStr = NULL;
    }

    return pcTmStr;
}

/**
 * @brief 校验路径字符串是否合法。
 * @details 校验路径是否为空指针、空字符串，以及是否包含非法字符（<>"|?*）。
 * @param[in] pcPth 待校验的路径字符串指针。
 * @return 校验结果。
 * @retval ER_SUC 路径合法。
 * @retval ER_SW_UNKN 路径非法或入参无效。
 * @see 扇入：erCrtDir、erInitLog。
 * @see 扇出：strlen、strchr、LogTr、LogErr。
 */
err erIsVldPth(const char* pcPth)
{
    const char* invalid_chars = "<>\"|?*";
    const char* pcChr = NULL;
    bl bInvChr = FALSE;
    err erRtn = ER_SW_UNKN;

    if((pcPth != NULL) && (strlen(pcPth) > 0))
    {
        for(pcChr = pcPth; (bInvChr == FALSE) && (*pcChr != '\0'); pcChr++)
        {
            if(strchr(invalid_chars, *pcChr))
            {
                bInvChr = TRUE;
            }
        }

        if(bInvChr == FALSE)
        {
            LogTr("Valid path.");

            erRtn = ER_SUC;
        }
        else
        {
            LogErr("Invalid path.");

            erRtn = ER_SW_UNKN;
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = ER_SW_UNKN;
    }

    return erRtn;
}

/**
 * @brief 获取全路径中的文件名。
 * @details 通过查找最后一个 '/' 符号来截取文件名。
 * @param[in] cpPath 文件全路径指针。
 * @return 文件名字符串指针。
 * @attention 该函数只适用于 Linux 平台，Windows 平台需将 '/' 改为 '\\'。
 */
char* pcGetFileNm(const char* const cpPath)
{
    char* pcFileNm = NULL;

    pcFileNm = strrchr(cpPath, '/');

    if(pcFileNm)
    {
        pcFileNm = pcFileNm + 1u;
    }
    else
    {
        pcFileNm = (char*)cpPath;
    }

    return pcFileNm;
}

/**
 * @brief 递归创建目录。
 * @details 若目录已存在且为目录类型则直接返回成功；若不存在则逐级创建缺失的中间目录。
 * @param[in] pcPth 目录路径字符串指针。
 * @return 创建结果。
 * @retval ER_SUC 目录创建成功或已存在。
 * @retval ER_SW_UNKN 路径非法、创建失败或路径被文件占用。
 * @see 扇入：erInitLog。
 * @see 扇出：strlen、erIsVldPth、LogInf、stat、LogTr、LogErr、strncpy、mkdir、LogScs。
 */
err erCrtDir(const char* pcPth)
{
    char acPth[PTH_MAX_SIZE] = {0};
    const char* pcChr = NULL;
    u16 u16PthSz = 0u;
    struct stat st = {0};
    err erRtn = ER_SW_UNKN;

    if((pcPth != NULL) && (strlen(pcPth) > 0) &&
       (strlen(pcPth) < PTH_MAX_SIZE) && (erIsVldPth(pcPth) == ER_SUC))
    {
        LogInf("Directory path is %s", pcPth);

        if(stat(pcPth, &st) == 0)
        {
            if(S_ISDIR(st.st_mode))
            {
                LogTr("The path already exists.");

                erRtn = ER_SUC;
            }
            else
            {
                LogErr("The path already exists but is not a directory.");

                erRtn = ER_SW_UNKN;
            }
        }
        else
        {
            if(errno == ENOENT)
            {
                LogTr("The path does not exist, create a new path.");

                u16PthSz = strlen(pcPth);
                erRtn = ER_SUC;

                for(pcChr = pcPth + 1; (pcChr - pcPth) <= u16PthSz; pcChr++)
                {
                    // if(*pcChr == '/')
                    if((*pcChr == '/') || (*pcChr == '\0'))
                    {
                        // 构建当前目录路径
                        strncpy(acPth, pcPth, pcChr - pcPth);
                        acPth[pcChr - pcPth] = '\0';
                        LogInf("Create directory %s", acPth);

                        if(stat(acPth, &st) == 0)
                        {
                            LogTr("The directory already exists.");
                        }
                        else
                        {
                            if(errno == ENOENT)
                            {
                                LogTr("create a new directory");

                                if(mkdir(acPth, 0755) == 0)
                                {
                                    LogScs("Directory [%s] created successfully", acPth);
                                }
                                else
                                {
                                    erRtn = ER_SW_UNKN;
                                    break;
                                }
                            }
                            else
                            {
                                LogErr("Directory access failed.");

                                erRtn = ER_SW_UNKN;
                                break;
                            }
                        }
                    }
                }
            }
            else
            {
                LogErr("Path access failed.");

                erRtn = ER_SW_UNKN;
            }
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = ER_SW_UNKN;
    }

    LogTr("Exit erCrtDir function.");

    return erRtn;
}

/**
 * @brief 初始化日志模块并打开日志文件。
 * @details 校验日志文件路径，创建所需目录后以追加方式打开日志文件。
 * @param[in] pcFilePth 日志文件完整路径指针。
 * @return 初始化结果。
 * @retval ER_SUC 日志文件成功打开。
 * @retval ER_SW_UNKN 路径非法、目录创建失败或文件打开失败。
 * @see 扇入：上层应用。
 * @see 扇出：strlen、erIsVldPth、LogInf、snprintf、strrchr、strcpy、erCrtDir、fopen、LogScs、LogErr、LogTr。
 */
err erInitLog(const char* pcFilePth)
{
    LogTr("Enter erInitLog function.");

    char acPth[PTH_MAX_SIZE] = {0};
    char* pcLstSlash = NULL;
    u16 u16FilePthSz = 0u;
    err erRtn = ER_SW_UNKN;

    if(pcFilePth != NULL)
    {
        LogInf("Log file path is %s", pcFilePth);

        u16FilePthSz = strlen(pcFilePth);

        if((u16FilePthSz > 0) &&
           (u16FilePthSz < PTH_MAX_SIZE) &&
           (erIsVldPth(pcFilePth) == ER_SUC))
        {
            snprintf(acPth, PTH_MAX_SIZE, "%s", pcFilePth);
            LogInf("Log directory path is %s", acPth);
            pcLstSlash = strrchr(acPth, '/');

            if(pcLstSlash != NULL)
            {
                *pcLstSlash = '\0';
            }
            else
            {
                strcpy(acPth, ".");
            }

            if(erCrtDir(acPth) == ER_SUC)
            {
                pfLogFile = fopen(pcFilePth, "a");

                if(pfLogFile != NULL)
                {
                    LogScs("The log file has been successfully opened.");

                    erRtn = ER_SUC;
                }
                else
                {
                    LogErr("Failed to open log file.");

                    erRtn = ER_SW_UNKN;
                }
            }
            else
            {
                LogErr("Log directory verification failed.");

                erRtn = ER_SW_UNKN;
            }
        }
        else
        {
            LogErr("Log path check error.");

            erRtn = ER_SW_UNKN;
        }
    }
    else
    {
        LogErr("Invalid function parameter.");

        erRtn = ER_SW_UNKN;
    }

    LogTr("Exit erInitLog function.");

    return erRtn;
}

/**
 * @brief 关闭日志模块，释放日志文件资源。
 * @see 扇出：fclose。
 */
void vidClsLog(void)
{
    if(pfLogFile != NULL)
    {
        fclose(pfLogFile);
        pfLogFile = NULL;
    }
}

/**
 * @brief 将格式化日志写入日志文件。
 * @param[in] pcFmt 格式化字符串指针。
 * @param[in] ... 可变参数列表。
 * @see 扇入：vidLog 宏。
 * @see 扇出：vfprintf、fflush。
 */
void vidLogPrintf(const char* pcFmt, ...)
{
    va_list args;
    va_start(args, pcFmt);

    // vprintf(pcFmt, args);

    if(pfLogFile != NULL)
    {
        vfprintf(pfLogFile, pcFmt, args);
        fflush(pfLogFile);
    }

    va_end(args);
}

/** @} */

#endif //LOG_H
