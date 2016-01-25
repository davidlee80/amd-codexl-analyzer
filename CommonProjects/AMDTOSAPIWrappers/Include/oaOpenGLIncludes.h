//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file oaOpenGLIncludes.h 
/// 
//=====================================================================

//------------------------------ oaOpenGLIncludes.h ------------------------------

#ifndef __OAOPENGLINCLUDES
#define __OAOPENGLINCLUDES

// Infra:
#include <AMDTBaseTools/Include/gtDefinitions.h>

// OpenGL "Per OS" definitions:
#include <AMDTOSAPIWrappers/Include/oaOpenGLOSSpecificDefinitions.h>

// OpenGL includes:
#if GR_BUILD_TARGET == GR_WINDOWS_OS

    #include <AMDTOSAPIWrappers/Include/GL/winXP/GL/gl.h>
    #include <AMDTOSAPIWrappers/Include/GL/winXP/GL/wglext.h>
    #include <AMDTOSAPIWrappers/Include/GL/glext.h>
    #include <AMDTOSAPIWrappers/Include/GL/glextremoved.h>
    #include <AMDTOSAPIWrappers/Include/GL/GRemdeyGLExtensions.h>

#elif GR_BUILD_TARGET == GR_LINUX_OS

    // Yaki 16/11/06:
    // This definition tells gl.h to don't include glext.h, enabling us to include
    // our glext.h version instead of the /usr/include/GL file:
    #define GL_GLEXT_LEGACY

    // Yaki 25/03/07:
    // GL_GLEXT_PROTOTYPES and GLX_GLXEXT_PROTOTYPES causes extension function's prototypes to be declared
    // in glext.h and glxext.h. This causes our function implementations to be externed as "C" functions (as
    // they should be)
    #define GL_GLEXT_PROTOTYPES 1
    #define GLX_GLXEXT_PROTOTYPES 1

    #if GR_LINUX_VARIANT == GR_GENERIC_LINUX_VARIANT
        // Yaki 15/6/2007:
        // We implement glXGetProcAddressARB. To have its decelaration included, we need to define GLX_GLXEXT_LEGACY.
        // This definition also tells glx.h to don't include glxext.h, enabling us to include our glxext.h version
        // instead of the /usr/include/GL file.

        #define GLX_GLXEXT_LEGACY


        #include <AMDTOSAPIWrappers/Include/GL/linux/GL/gl.h>
        #include <AMDTOSAPIWrappers/Include/GL/glext.h>
        #include <AMDTOSAPIWrappers/Include/GL/glextremoved.h>
        #include <AMDTOSAPIWrappers/Include/GL/linux/GL/glext_additions.h>
        #include <AMDTOSAPIWrappers/Include/GL/linux/GL/glx.h>
        #include <AMDTOSAPIWrappers/Include/GL/linux/GL/glxext.h>
        #include <AMDTOSAPIWrappers/Include/GL/GRemdeyGLExtensions.h>

    #elif GR_LINUX_VARIANT == GR_MAC_OS_X_LINUX_VARIANT

        #include <AMDTOSAPIWrappers/Include/GL/mac/OpenGL/gl.h>
        #include <AMDTOSAPIWrappers/Include/GL/glext.h>
        #include <AMDTOSAPIWrappers/Include/GL/glextremoved.h>
        #include <AMDTOSAPIWrappers/Include/GL/mac/OpenGL/glext_additions.h>
        #include <AMDTOSAPIWrappers/Include/GL/GRemdeyGLExtensions.h>

        // CGL is not used in the iPhone:
        #ifndef _GR_IPHONE_BUILD
            #include <AMDTOSAPIWrappers/Include/GL/mac/OpenGL/OpenGL.h>
        #endif

    #elif GR_LINUX_VARIANT == GR_ANDROID_LINUX_VARIANT
        #include <AMDTOSAPIWrappers/Include/GLES/egl.h>
    #else
        #error Unknown Linux variant!
    #endif
#else
    #error Unknown build target!
#endif


// OpenGL ES is currently supported only on Windows and Mac:
#if (GR_BUILD_TARGET == GR_WINDOWS_OS)

    // Windows OpenGL ES includes:
    #include <AMDTOSAPIWrappers/Include/GLES/gl-ESAdditions.h>
    #include <AMDTOSAPIWrappers/Include/GLES/glext-ESAdditions.h>
    #include <AMDTOSAPIWrappers/Include/GLES/egl.h>

#elif ((GR_BUILD_TARGET == GR_LINUX_OS) && (GR_LINUX_VARIANT == GR_MAC_OS_X_LINUX_VARIANT))

    // iPhone OpenGL ES includes:
    #include <AMDTOSAPIWrappers/Include/GLES/gl-ESAdditions.h>
    #include <AMDTOSAPIWrappers/Include/GLES/iPhone/glext-iPhoneAdditions.h>

#elif ((GR_BUILD_TARGET == GR_LINUX_OS) && (GR_LINUX_VARIANT == GR_GENERIC_LINUX_VARIANT))
    // A few defintions copied from GLES gl.h to enable building the Linux version:
    #define GL_TEXTURE_CROP_RECT_OES          0x8B9D
#endif

#endif  // __OAOPENGLINCLUDES


