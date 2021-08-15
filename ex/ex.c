/**
 * @file ex.c
 * @brief 源文件格式模板。
 * @details 无
 * @author Calm
 * @date 2021-07-11
 * @version v1.0.0
 * @copyright Calm
 */

#include "hdr.h"

#ifdef EX_H

#if (_PROJECT_NAME_ == _PROJECT_NAME_) && (_PROJECT_NUM_ == _PROJECT_NUM_)

/*****************************************************************************
 *宏定义                                                                     *
 *****************************************************************************/
//=============================================================================
//分类
//-----------------------------------------------------------------------------
//子分类
#define DEFINE_EX                  0u                   //!< 宏定义示例。

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
    EnumEx1 = 1u //!< 枚举成员定义示例。
    EnumEx1 = 2u //!< 枚举成员定义示例。
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
    Bool bStructEx;   //!< 结构体成员定义示例。
    Byte byStructEx;  //!< 结构体成员定义示例。
    Word wStructEx;   //!< 结构体成员定义示例。
    DWord dwStructEx; //!< 结构体成员定义示例。
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
    Byte byUnionEx; //!< 结构体成员定义示例。

    struct
    {
        Byte byUnionEx1 : 1u; //!< 结构体成员定义示例。
        Byte byUnionEx2 : 1u; //!< 结构体成员定义示例。
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
//静态函数
//-----------------------------------------------------------------------------
//内敛函数
static __forceinline void StcForceInlineFunEx(void);
static __inline void StcInlineFunEx(void);

//-----------------------------------------------------------------------------
//普通函数
static void StcFunEx(void);

/*****************************************************************************
 *变量定义                                                                   *
 *****************************************************************************/
//=============================================================================
//全局变量
Byte g_byGlVar = 0u;
const Byte gc_byGlCstVar = 0u;

//=============================================================================
//静态变量
static Byte s_byLclVar = 0u;

/*****************************************************************************
 *函数定义                                                                   *
 *****************************************************************************/
//=============================================================================
//静态函数
//-----------------------------------------------------------------------------
//内敛函数
/**
 * @fn static __forceinline void StcForceInlineFunEx(void)
 * @brief 静态内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __forceinline void StcForceInlineFunEx(void)
{
}

/**
 * @fn static __inline void StcInlineFunEx(void)
 * @brief 静态内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static __inline void StcInlineFunEx(void)
{
}

//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn static void StcFunEx(void)
 * @brief 静态函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
static void StcFunEx(void)
{
}

//=============================================================================
//全局函数
//-----------------------------------------------------------------------------
//内敛函数
/**
 * @fn __forceinline void ForceInlineFunEx(void)
 * @brief 内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
__forceinline void ForceInlineFunEx(void)
{
}

/**
 * @fn __inline void InlineFunEx(void)
 * @brief 内敛函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
__inline void InlineFunEx(void)
{
}

//-----------------------------------------------------------------------------
//普通函数
/**
 * @fn void GlFunEx(void)
 * @brief 全局函数定义示例。
 * @details 无
 * @param void
 * @return void
 * @note 无
 * @attention 无
 */
void GlFunEx(void)
{
    const Byte c_byCstVar = 0u;
    static Byte s_byStcVar = 0u;
}

#endif //_PROJECT_NAME_ _PROJECT_NUM_
#endif //EX_H
