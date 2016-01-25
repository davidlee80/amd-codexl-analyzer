//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcVulkanStatisticsParser.h 
/// 
//=====================================================================
#ifndef kcVulkanStatisticsParser_h__
#define kcVulkanStatisticsParser_h__

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcIStatisticsParser.h>

class kcVulkanStatisticsParser :
	public IStatisticsParser
{
public:
	kcVulkanStatisticsParser();
	virtual ~kcVulkanStatisticsParser();

	virtual bool ParseStatistics(const gtString& statisticsFilePath, beKA::AnalysisData& parsedStatistics) override;

};

#endif // kcVulkanStatisticsParser_h__
