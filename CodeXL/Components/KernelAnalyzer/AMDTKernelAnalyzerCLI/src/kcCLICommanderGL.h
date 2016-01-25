//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommanderGL.h 
/// 
//=====================================================================
#ifndef _kcCLICommanderGL_H_
#define _kcCLICommanderGL_H_

#include <AMDTKernelAnalyzerCLI/src/kcCLICommander.h>

class kcCLICommanderGL : public kcCLICommander
{
public:
    kcCLICommanderGL(void);
    virtual ~kcCLICommanderGL(void);

    /// List the asics as got from device
    void ListAsics(Config& config, LoggingCallBackFunc_t callback);

    /// list the driver version
    void Version(Config& config, LoggingCallBackFunc_t callback);

    /// Output multiple commands for all commands that requires compilation: GetBinary, GetIL, GetISA, GetAnlysis, GetMetadata, GetDebugIL,ListKernels
    void RunCompileCommands(const Config& config, LoggingCallBackFunc_t callback);

private:
    std::vector<GDT_GfxCardInfo> m_glDefaultAsicsList;
    Backend* m_pBackend;

private:
    bool Init(const Config& config, LoggingCallBackFunc_t callback);
    void InitRequestedAsicList(const Config& config);
    bool Compile(const Config& config, const GDT_GfxCardInfo& gfxCardInfo, const std::string& sDevicenametoLog);
    void CreateStatisticsFile(const Config& config, const std::map<std::string, beKA::AnalysisData>& analysisData);

};

#endif