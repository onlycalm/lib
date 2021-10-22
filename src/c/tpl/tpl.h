/**
 * @file tpl.h
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

#if (_PROJECT_NAME_ == _PROJECT_NAME_) && (_PROJECT_NUM_ == _PROJECT_NUM_)

/*****************************************************************************
 *类型定义                                                                   *
 *****************************************************************************/
typedef int8_t s8; //类型定义示例。

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//分类
//-----------------------------------------------------------------------------
//子分类
#define DEFINE_TPL                 0u                   //!<宏定义示例。

//=============================================================================
//宏函数定义

/*****************************************************************************
 *枚举定义                                                                   *
 *****************************************************************************/
/**
 * @enum EnumEx
 * @brief 枚举定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef enum EnumEx
{
    EnumEx1 = 1u, //!<枚举成员定义示例。
    EnumEx1 = 2u  //!<枚举成员定义示例。
}EEnumEx;

/*****************************************************************************
 *结构体定义                                                                 *
 *****************************************************************************/
/**
 * @struct StructEx
 * @brief 结构体定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef struct StructEx
{
    Bool bStructEx;   //!<结构体成员定义示例。
    Byte byStructEx;  //!<结构体成员定义示例。
    Word wStructEx;   //!<结构体成员定义示例。
    DWord dwStructEx; //!<结构体成员定义示例。
}STStructEx;

/*****************************************************************************
 *联合定义                                                                   *
 *****************************************************************************/
/**
 * @union UnionEx
 * @brief 联合定义示例。
 * @details 无
 * @note 无
 * @attention 无
 */
typedef union UnionEx
{
    Byte byUnionEx; //!<结构体成员定义示例。

    struct
    {
        Byte byUnionEx1 : 1u; //!<结构体成员定义示例。
        Byte byUnionEx2 : 1u; //!<结构体成员定义示例。
    };
}UUnionEx;

/*****************************************************************************
 *变量声明                                                                   *
 *****************************************************************************/
//=============================================================================
//全局变量
extern Byte g_byGlVar;
extern const Byte gc_byGlCstVar;

/*****************************************************************************
 *函数声明                                                                   *
 *****************************************************************************/
//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//内敛函数
extern __forceinline void ForceInlineFunEx(void);
extern __inline void InlineFunEx(void);

//-----------------------------------------------------------------------------
//普通函数
extern void GlFunEx(void);

#endif //_PROJECT_NAME_ _PROJECT_NUM_
#endif //TPL_H
