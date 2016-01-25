//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file osBundle.h 
/// 
//=====================================================================

//------------------------------ osBundle.h ------------------------------

#ifndef __OSBUNDLE
#define __OSBUNDLE

// MAC OS
#if ((GR_BUILD_TARGET == GR_LINUX_OS) && (GR_LINUX_VARIANT == GR_MAC_OS_X_LINUX_VARIANT))
    // Forward declarations:
    typedef struct __CFBundle* CFBundleRef;

    // Local:
    #include <AMDTOSWrappers/Include/osOSWrappersDLLBuild.h>
    #include <AMDTOSWrappers/Include/osOSDefinitions.h>

    OS_API osProcedureAddress osGetOpenGLFrameworkFunctionAddress(const char* procName);
    bool osGetSystemOpenGLFrameworkBundle(CFBundleRef& refOpenGLFrameworkBundle);
    OS_API void osSetOpenGLESFrameworkPath(const char* pFrameworkPath);
    OS_API const char* osGetOpenGLESFrameworkPath();

#endif

#endif  // __OSBUNDLE
