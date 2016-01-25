//=====================================================================
// Copyright 2008-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================

/**
 *  @file     cal_private.h
 *  @brief    Private CAL Interface Header
 *  @version  1.00.0 Beta
 */



#ifndef __CAL_PRIVATE_H__
#define __CAL_PRIVATE_H__

#include "cal.h"

#ifdef __cplusplus
extern "C" {
#define CALAPI
#else
#define CALAPI extern
#endif

#ifdef _WIN32
#define CALAPIENTRY  __stdcall
#else
#define CALAPIENTRY
#endif
#ifndef CALAPIENTRYP
#define CALAPIENTRYP CALAPIENTRY *
#endif

// Private parts of enums that will eventually move into public

/*============================================================================
 * CAL Runtime Interface
 *============================================================================*/

/** CAL resource map flags **/
typedef enum CALresmapflagsEnum {
    CAL_RESMAP_READWRITE  = 0, /**< used for read and write */
    CAL_RESMAP_READ       = 1, /**< used for readonly */
    CAL_RESMAP_WRITE      = 2, /**< used for writeonly */
} CALresmapflags;

/** CAL resource allocation flags **/
typedef enum CALresallocflagsPrivateEnum {
    CAL_RESALLOC_PERSISTENT     = (1 << 31), /**< direct mappable memory */
} CALresallocflagsPrivate;

typedef enum CALresultPrivateEnum {
    CAL_RESULT_INVALID_THREAD    = 11, /**< Runtime context is not in the same thread as creation */
} CALresultPrivate;

typedef enum CALmemcopyflagsEnum
{
    CAL_MEMCOPY_DEFAULT = 0, /**< default CAL behavior of partial sync */
    CAL_MEMCOPY_SYNC    = 1, /**< used to synchronize with the specified CAL context */
    CAL_MEMCOPY_ASYNC   = 2, /**< used to indicate completely asynchronous behavior */
} CALmemcopyflags;

typedef struct CALfuncExtendedInfoRec
{
    CALuint     struct_size;         /**< Client filled out size of CALdeviceattribs struct */
    CALfuncInfo funcInfo;            /**< calFuncInfo*/
    CALboolean  bLDSisUsed;          /**< LDS is used in func */
} CALfuncExtendedInfo;

/*----------------------------------------------------------------------------
 * CAL Context Functions
 *----------------------------------------------------------------------------*/



#ifdef __cplusplus
}      /* extern "C" { */
#endif


#endif /* __CAL_PRIVATE_H__ */



