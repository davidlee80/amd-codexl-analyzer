//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommander.h 
/// 
//=====================================================================
#ifndef _KCCLICOMMANDER_H_
#define _KCCLICOMMANDER_H_

// C++.
#include <string>
#include <set>

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcConfig.h>
#include <AMDTKernelAnalyzerCLI/src/kcDataTypes.h>

using namespace std;

#define IsaSuffix "amdisa"
#define MetadataSuffix "amdMetadata"
#define DebugilSuffix "amdDebugil"

/// This is the Commander interface 
class kcCLICommander
{
public:

    virtual ~kcCLICommander() {};

    /// List the asics as got from device
    virtual void ListAsics(Config& config, LoggingCallBackFunc_t callback) = 0;

    /// list the driver version
    virtual void Version(Config& config, LoggingCallBackFunc_t callback) = 0;

    /// Output multiple commands for all commands that requires compilation: GetBinary, GetIL, GetISA, GetAnlysis, GetMetadata, GetDebugIL,ListKernels
    virtual void RunCompileCommands(const Config& config, LoggingCallBackFunc_t callback) = 0;


protected: // functions
    LoggingCallBackFunc_t m_LogCallback;

    /// Logging callback type.
    bool LogCallBack(const std::string& theString)
    {
        bool bRet = false;
        if (m_LogCallback)
        {
            m_LogCallback(theString);
            bRet = true;
        }
        return bRet;
    }
};



#endif


