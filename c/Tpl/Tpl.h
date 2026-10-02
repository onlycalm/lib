/**
 * @file Tpl.h
 * @brief 头文件格式模板。
 * @details 无
 * @author Calm
 * @date 2021-07-11
 * @version v1.0.0
 * @copyright Calm
 */

#ifndef TPL_H
#define TPL_H

#include "hdr.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * 类型定义                                                                  *
 *****************************************************************************/
typedef int8_t s8; // 类型定义示例。

/* ===== 宏定义 ===== */
/* == 分类 == */
/* -- 子分类 -- */
#define DEFINE_TPL 0u //!< 宏定义示例。

/* ===== 宏函数定义 ===== */
/**
 * @def 宏函数名。
 * @brief 简述宏函数功能。
 * @param[in] 参数名 参数注解。
 * @param[out] 参数名 参数注解。
 * @param[in, out] 参数名 参数注解。
 * @return 函数返回注解。
 * @retval 对返回值的说明。
 */
#define vidMacroFun() \
do{ /* 注释 */     \
}while(0u)

/* ===== 枚举定义 ===== */
/**
 * @enum enEnumEx
 * @brief 枚举定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef enum enEnumEx
{
    EnumEx1 = 1u, //!< 枚举成员定义示例。
    EnumEx2 = 2u, //!< 枚举成员定义示例。
} enEnumEx;

/* ===== 结构体定义 ===== */
/**
 * @struct stStructEx
 * @brief 结构体定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef struct stStructEx
{
    bl bStructEx;  //!< 结构体成员定义示例。
    u8 u8StructEx; //!< 结构体成员定义示例。
} stStructEx;

/* ===== 联合定义 ===== */
/**
 * @union unUnionEx
 * @brief 联合定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef union unUnionEx
{
    u8 u8UnionEx; //!< 结构体成员定义示例。

    struct
    {
        u8 b2UnionEx1 : 2u; //!< 结构体成员定义示例。
        u8 b4UnionEx2 : 4u; //!< 结构体成员定义示例。
        u8 b2Rsv      : 2u; //!< 结构体成员定义示例。
    };
} unUnionEx;

/* ===== 变量声明 ===== */
/* == 全局变量 == */
extern u8 g_u8GlVar;
extern const u8 g_ku8GlCstVar;
extern enEnumEx g_eEnumEx;
extern stStructEx g_tStructEx;
extern unUnionEx g_uUnionEx;

/* ===== 函数声明 ===== */
/* == 全局函数 == */
/* -- 弱函数 -- */
extern void vidWeakFunEx(void);

/* -- 普通函数 -- */
extern void vidGlFunEx(void);

/* ===== 函数定义 ===== */
/* == 全局函数 == */
/* -- 内敛函数 -- */
/**
 * @brief 内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
STC_FRC_INLINE void vidStcForceInlineFunEx(void)
{
}

/**
 * @brief 内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
STC_INLINE void vidStcInlineFunEx(void)
{
}

#ifdef __cplusplus
}
#endif

#endif // TPL_H
