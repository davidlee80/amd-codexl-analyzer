//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beStaticIsaAnalyzer.h 
/// 
//=====================================================================
#ifndef beStaticIsaAnalyzer_h__
#define beStaticIsaAnalyzer_h__

// Infra.
#include <AMDTBaseTools/Include/gtString.h>

// Local.
#include <AMDTBackEnd/Include/beInclude.h>

namespace beKA
{
	class KA_BACKEND_DECLDIR beStaticIsaAnalyzer
	{
	public:

		/// Perform live register analysis on the ISA contained in the given file,
		/// and dump the analysis output to another file.
		/// Params:
		///		isaFileName: the file name that contains the ISA to be analyzed.
		///		outputFileName: the output file name (will contain the analysis output).
		/// Returns: true if the operation succeeded, false otherwise.
		static beStatus PerformLiveRegisterAnalysis(const gtString& isaFileName, const gtString& outputFileName);

	private:
		// No instances.
		beStaticIsaAnalyzer(const beStaticIsaAnalyzer& other);
		beStaticIsaAnalyzer();
		~beStaticIsaAnalyzer();
	};
}
#endif // beStaticIsaAnalyzer_h__