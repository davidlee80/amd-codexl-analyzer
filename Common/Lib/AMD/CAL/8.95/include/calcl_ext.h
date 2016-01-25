
//=====================================================================
// Copyright 2007-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================


#ifndef __CALCL_EXT_H__
#define __CALCL_EXT_H__

#include "calcl.h"

#ifdef __cplusplus
extern "C" {
#define CALAPI
#else
#define CALAPI extern
#endif

typedef enum CALCLextidEnum
{
    CALCL_EXT_XXXX        = 0x1001,   /* undefined */
} CALCLextid;

typedef CALvoid* CALCLextproc;

CALAPI CALresult CALAPIENTRY calclExtSupported(CALCLextid extid);
CALAPI CALresult CALAPIENTRY calclExtGetProc(CALCLextproc* proc, CALCLextid extid, const CALchar* procname);


#ifdef __cplusplus
}      /* extern "C" { */
#endif

#endif


