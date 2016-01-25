//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file oaDriver.cpp 
/// 
//=====================================================================

//------------------------------ oaDriver.cpp ------------------------------

// Local - OSDriver:
#include <AMDTOSAPIWrappers/Include/oaDriver.h>
#include <AMDTOSWrappers/Include/osProcess.h>

// Definitions:
struct ADLVersionsInfo
{
    char strDriverVer[256];        //Driver Release (Packaging) Version (e.g. 8.71-100128n-094835E-ATI).
    char strCatalystVersion[256];  //Catalyst Version(e.g. "10.1").
    char strCatalystWebLink[256];  //Web link to an XML file with information about the latest AMD
};

typedef unsigned int (CL_API_CALL* CALGETVERSION_PROC)(unsigned int* major, unsigned int* minor, unsigned int* imp);
typedef int(*DRIVERGETVERSION_PROC)(ADLVersionsInfo* versionInfo);

typedef void* (CL_API_CALL* ADL_MAIN_MALLOC_CALLBACK)(int);
typedef int(*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int(*ADL_MAIN_CONTROL_DESTROY)();

// POSIX:
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>

// Infra:
#include <AMDTBaseTools/Include/gtAssert.h>
#include <AMDTBaseTools/Include/gtDefinitions.h>
#include <AMDTOSWrappers/Include/osDebugLog.h>
#include <AMDTOSWrappers/Include/osFilePath.h>
#include <AMDTOSWrappers/Include/osModule.h>
#include <AMDTOSWrappers/Include/osOSDefinitions.h>

// Linux-only headers:
#if (GR_BUILD_TARGET == GR_LINUX_OS) && (GR_LINUX_VARIANT == GR_GENERIC_LINUX_VARIANT)
    #include <elf.h>
#endif

// Local:
#include <AMDTOSAPIWrappers/Include/oaStringConstants.h>

void* ADL_Main_Memory_Alloc(int iSize)
{
    void* lpBuffer = malloc(iSize);
    return lpBuffer;
}

// ---------------------------------------------------------------------------
// Name:        oaGetCalVersion
// Description: Dynamically load aticalrt.dll module and using calGetVersion
//              get the cal version
// Return Val:  bool - Success / failure.
// Author:      AMD Developer Tools Team
// Date:        4/5/2011
// ---------------------------------------------------------------------------
OA_API bool oaGetCalVersion(gtString& calVersion)
{
    // TO_DO: not implemented
    calVersion.makeEmpty();

    return false;
}

// ---------------------------------------------------------------------------
// Name:        oaGetCalVersion
// Description: Dynamically load atiadlxx.dll module and using ADL_Graphics_Versions_Get
//              driverVersion - return value with the catalyst version
// Return Val:  bool - Success / failure.
// Author:      AMD Developer Tools Team
// Date:        4/5/2011
// ---------------------------------------------------------------------------
OA_API gtString oaGetDriverVersion(int& driverError)
{
    driverError = OA_DRIVER_UNKNOWN;
    gtString driverVersion;

    // Get the cal dll module:
    osFilePath driverModulePath(osFilePath::OS_SYSTEM_DIRECTORY, OS_ATI_CALXY_DRIVER_DLL_NAME, OS_MODULE_EXTENSION);
    osModuleHandle driverModuleHandle = NULL;

    bool rcLoadedModule = osLoadModule(driverModulePath, driverModuleHandle, NULL, false);

    if (!rcLoadedModule)
    {
        // Try to get the ATI cal XX driver dll file:
        driverModulePath.setFileName(OS_ATI_CALXX_DRIVER_DLL_NAME);

        // Load the module:
        rcLoadedModule = osLoadModule(driverModulePath, driverModuleHandle, NULL, false);
    }

    // if module not found so far use the catalyst directory (usually in Ubuntu it is needed)
    if (!rcLoadedModule)
    {
        gtString systemDir = driverModulePath.fileDirectoryAsString();
        systemDir.append(L"/");
        systemDir.append(OS_AMD_DRIVER_DIRECTORY);
        driverModulePath.setFileDirectory(systemDir);

        // Load the module:
        rcLoadedModule = osLoadModule(driverModulePath, driverModuleHandle, NULL, false);

        if (!rcLoadedModule)
        {
            driverModulePath.setFileName(OS_ATI_CALXY_DRIVER_DLL_NAME);

            // Load the module:
            rcLoadedModule = osLoadModule(driverModulePath, driverModuleHandle, NULL, false);
        }
    }

    GT_IF_WITH_ASSERT(rcLoadedModule && (driverModuleHandle != NULL))
    {
        // Get the procedure for the cal function:
        osProcedureAddress driverVersionFunction = NULL;
        osProcedureAddress adlCreateFunction = NULL;
        osProcedureAddress adlDestroyFunction = NULL;

        bool rc = osGetProcedureAddress(driverModuleHandle, OA_STR_ADL_DRIVER_VERSION_FUNCTION, driverVersionFunction);
        bool createrc = osGetProcedureAddress(driverModuleHandle, OA_STR_ADL_DRIVER_CREATE_FUNCTION, adlCreateFunction);
        bool destroyrc = osGetProcedureAddress(driverModuleHandle, OA_STR_ADL_DRIVER_DESTROY_FUNCTION, adlDestroyFunction);
        GT_IF_WITH_ASSERT(rc && createrc && destroyrc && (driverVersionFunction != NULL) && (adlCreateFunction != NULL) && (adlDestroyFunction != NULL))
        {
            // Init ADL:
            ADL_MAIN_CONTROL_CREATE adlCreateFunctionWrapper = (ADL_MAIN_CONTROL_CREATE)adlCreateFunction;
            adlCreateFunctionWrapper(ADL_Main_Memory_Alloc, 1);

            // Check the version:
            DRIVERGETVERSION_PROC driverGetVersionWrapper = (DRIVERGETVERSION_PROC)driverVersionFunction;
            ADLVersionsInfo driverInfo;
            unsigned int result = driverGetVersionWrapper(&driverInfo);

            // When the result is 1, it means that we have a warning, but we can ignore it:
            GT_IF_WITH_ASSERT((result == 0) || (result == 1))
            {
                driverVersion.fromASCIIString(driverInfo.strCatalystVersion);

                if (driverVersion.isEmpty())
                {
                    driverError = OA_DRIVER_VERSION_EMPTY;
                }
                else
                {
                    driverError = OA_DRIVER_OK;
                }
            }

            // Destroy ADL:
            ADL_MAIN_CONTROL_DESTROY adlDestroyFunctionWrapper = (ADL_MAIN_CONTROL_DESTROY)adlDestroyFunction;
            adlDestroyFunctionWrapper();
        }


        osReleaseModule(driverModuleHandle);
    }
    else
    {
        driverError = OA_DRIVER_NOT_FOUND;
    }

    // Currently, staging driver does not report the catalyst version properly.
    // This workaround retrieves the Catalyst version via the shell, rather than via the driver API.
    if (driverVersion.isEmpty())
    {
        const char* COMMAND_STR = "dmesg|grep module|grep fglrx";
        gtString outStr;
        gtString versionFromFglrx;
        osExecAndGrabOutput(COMMAND_STR, false, outStr);
        if (!outStr.isEmpty())
        {
            int fglrxPos = outStr.reverseFind(L" - fglrx ");
            if (fglrxPos != -1)
            {
                int verPos = outStr.find(L" [", fglrxPos);
                if (verPos != -1)
                {
                    outStr.getSubString(fglrxPos, verPos, versionFromFglrx);
                    versionFromFglrx = versionFromFglrx.trim();
                    if (!versionFromFglrx.isEmpty())
                    {
                        verPos = versionFromFglrx.reverseFind(L" ");
                        versionFromFglrx.getSubString(verPos + 1, versionFromFglrx.length() - 1, versionFromFglrx);
                        if (!versionFromFglrx.isEmpty())
                        {
                            driverVersion = versionFromFglrx;
                            driverError = OA_DRIVER_OK;
                        }
                    }
                }
            }
        }
    }

    return driverVersion;
}


