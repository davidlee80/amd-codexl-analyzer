//=====================================================================
// Copyright 2011-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcMain.cpp 
/// \brief   Command line interpreter KernelAnalyzer main.
/// 
//=====================================================================
#include <map>
#include <utility>
#include <sstream>
#include <boost/algorithm/string.hpp>

// Infra.
#include <AMDTOSWrappers/Include/osEnvironmentVariable.h>
#include <AMDTOSWrappers/Include/osProcess.h>

// Backend.
#include <AMDTBackEnd//Include/beProgramBuilderOpenCL.h>

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcConfig.h>
#include <AMDTKernelAnalyzerCLI/src/kcParseCmdLine.h>
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderCL.h>
#include <AMDTKernelAnalyzerCLI/src/kcCliStringConstants.h>
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderGL.h>
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderOpenGL.h>
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderVulkan.h>

#ifdef _WIN32
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderDX.h>
#endif



using namespace beKA;
static std::ostream& s_Log = cout;

static void loggingCallback(const string& s)
{
    s_Log << s.c_str();
}


int main(int argc, char* argv[])
{
#ifdef _WIN64
    // Enable 64-bit build for OpenCL.
    osEnvironmentVariable envVar64Bit(STR_OCL_ENV_VAR_GPU_FORCE_64BIT_PTR_NAME, STR_OCL_ENV_VAR_GPU_FORCE_64BIT_PTR_VALUE);
    osSetCurrentProcessEnvVariable(envVar64Bit);
#endif // _WIN64

    Config config;
    bool bCont = ParseCmdLine(argc, argv, config);
    if (!bCont)
    {
        config.m_RequestedCommand = Config::ccInvalid;
    }

    // do requested work
    kcCLICommander* pMyCommander = nullptr;
    if (config.m_SourceLanguage == SourceLanguage_OpenCL)
    {
        pMyCommander = reinterpret_cast<kcCLICommander*>(new kcCLICommanderCL);
    }

#ifdef _WIN32
    else if ((config.m_SourceLanguage == SourceLanguage_HLSL) || 
		(config.m_SourceLanguage == SourceLanguage_DXasm) || 
		(config.m_SourceLanguage == SourceLanguage_DXasmT))
    {
        pMyCommander = reinterpret_cast<kcCLICommander*>(new kcCLICommanderDX);
    }
#endif

    else if (config.m_SourceLanguage == SourceLanguage_GLSL)
    {
        pMyCommander = reinterpret_cast<kcCLICommander*>(new kcCLICommanderGL);
    }
	else if (config.m_SourceLanguage == SourceLanguage_GLSL_OpenGL)
	{
		pMyCommander = reinterpret_cast<kcCLICommander*>(new kcCLICommanderOpenGL);
	}
	else if (config.m_SourceLanguage == SourceLanguage_GLSL_Vulkan)
	{
		pMyCommander = reinterpret_cast<kcCLICommander*>(new kcCLICommanderVulkan);
	}

    if (pMyCommander == nullptr)
    {
        config.m_RequestedCommand = Config::ccInvalid;
    }
    
    switch (config.m_RequestedCommand)
    {
    case Config::ccHelp:
        break;
    case Config::ccCompile:
    case Config::ccListKernels:
        pMyCommander->RunCompileCommands(config, loggingCallback);
        break;
    case Config::ccListAsics:
        pMyCommander->ListAsics(config, loggingCallback);
        break;
    case Config::ccVersion:
        pMyCommander->Version(config, loggingCallback);
        break;
    case Config::ccInvalid:
        s_Log << STR_ERR_NO_VALID_CMD_DETECTED << endl;
        break;
    }

    delete pMyCommander;

    return 0;
}
