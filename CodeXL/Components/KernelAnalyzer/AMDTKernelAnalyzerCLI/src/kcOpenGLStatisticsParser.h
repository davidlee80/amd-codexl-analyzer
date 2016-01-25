//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcOpenGLStatisticsParser.h 
/// 
//=====================================================================
#ifndef kcOpenGLStatisticsParser_h__
#define kcOpenGLStatisticsParser_h__

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcIStatisticsParser.h>

class kcOpenGLStatisticsParser :
	public IStatisticsParser
{
public:
	kcOpenGLStatisticsParser();
	virtual ~kcOpenGLStatisticsParser();

	virtual bool ParseStatistics(const gtString& scStatistics, beKA::AnalysisData& statisticsAsCsv) override;

};

#endif // kcOpenGLStatisticsParser_h__
