//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommanderDX.h 
/// 
//=====================================================================
#ifndef _kcCLICommanderDX_H_
#define _kcCLICommanderDX_H_

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcCLICommander.h>
#include <AMDTKernelAnalyzerCLI/src/kcDataTypes.h>

class kcCLICommanderDX : public kcCLICommander
{
public:
	kcCLICommanderDX(void);
    virtual ~kcCLICommanderDX(void);

	/// List the asics as got from device
	void ListAsics(Config& config, LoggingCallBackFunc_t callback);

	/// list the driver version
	void Version(Config& config, LoggingCallBackFunc_t callback);

	void RunCompileCommands(const Config& config, LoggingCallBackFunc_t callback);

private: //functions
	bool Init(const Config& config, LoggingCallBackFunc_t callback);
	void InitRequestedAsicList(const Config& config);
	bool Compile(const Config& config, GDT_GfxCardInfo& gfxCardInfo, string sDevicenametoLog);
	bool WriteAnalysisDataForDX(const Config& config, const std::vector<AnalysisData>& AnalysisDataVec, const std::vector<string>& DeviceAnalysisDataVec, const std::string& sAnalysisFile, std::stringstream& log);

private: //members
	
	std::vector<GDT_GfxCardInfo> m_dxDefaultAsicsList;

	Backend* be;

};

#endif