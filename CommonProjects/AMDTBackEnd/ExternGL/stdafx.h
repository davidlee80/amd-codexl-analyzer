//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#ifdef _WINDOWS
    #pragma once

    #include "targetver.h"

    //#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
    // Windows Header Files:
    #include <windows.h>
    #include <tchar.h>
#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

#include <string>
#include <AMDTBackEnd/Include/GL/glew.h>
#include <assert.h>
#include "scl_types.h"



#define GL_PROGRAM_BUILD_PARAMETER_AMD                    0x9400 // 
#define GL_BUILD_DEBUG_AMD                                0x0001
#define GL_BUILD_ILBIN_AMD                                0x0040

//#include "cm_math.h"




// TODO: reference additional headers your program requires here
