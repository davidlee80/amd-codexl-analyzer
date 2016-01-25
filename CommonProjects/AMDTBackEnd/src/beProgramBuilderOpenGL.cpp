//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beProgramBuilderOpenGL.cpp 
/// 
//=====================================================================

// C++.
#include <sstream>

// Infra.
#include <AMDTBaseTools/Include/gtAssert.h>
#include <AMDTOSWrappers/Include/osDirectory.h>
#include <AMDTOSWrappers/Include/osProcess.h>

// Local.
#include <AMDTBackEnd/Include/beProgramBuilderOpenGL.h>
#include <AMDTBackEnd/Include/beUtils.h>

// Device info.
#include <DeviceInfoUtils.h>

// Internally-linked utilities.
static bool GetVirtualContextPath(std::string& virtualContextPath)
{
#ifdef __linux
	virtualContextPath = "./VirtualContext";
#elif _WIN32
	virtualContextPath = "x86\\VirtualContext.exe";
#else
	virtualContextPath = "x64\\VirtualContext.exe";
#endif
	return true;
}

beProgramBuilderOpenGL::beProgramBuilderOpenGL()
{

}

beProgramBuilderOpenGL::~beProgramBuilderOpenGL()
{
}

beKA::beStatus beProgramBuilderOpenGL::GetKernels(const std::string& device, std::vector<std::string>& kernels)
{
	GT_UNREFERENCED_PARAMETER(device);
	GT_UNREFERENCED_PARAMETER(kernels);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_Invalid;
}

beKA::beStatus beProgramBuilderOpenGL::GetBinary(const std::string& device, const beKA::BinaryOptions& binopts, std::vector<char>& binary)
{
	GT_UNREFERENCED_PARAMETER(device);
	GT_UNREFERENCED_PARAMETER(binopts);
	GT_UNREFERENCED_PARAMETER(binary);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_Invalid;
}

beKA::beStatus beProgramBuilderOpenGL::GetKernelILText(const std::string& device, const std::string& kernel, std::string& il)
{
	GT_UNREFERENCED_PARAMETER(device);
	GT_UNREFERENCED_PARAMETER(kernel);
	GT_UNREFERENCED_PARAMETER(il);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_Invalid;
}

beKA::beStatus beProgramBuilderOpenGL::GetKernelISAText(const std::string& device, const std::string& kernel, std::string& isa)
{
	GT_UNREFERENCED_PARAMETER(device);
	GT_UNREFERENCED_PARAMETER(kernel);
	GT_UNREFERENCED_PARAMETER(isa);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_Invalid;
}

beKA::beStatus beProgramBuilderOpenGL::GetStatistics(const std::string& device, const std::string& kernel, beKA::AnalysisData& analysis)
{
	GT_UNREFERENCED_PARAMETER(device);
	GT_UNREFERENCED_PARAMETER(kernel);
	GT_UNREFERENCED_PARAMETER(analysis);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_Invalid;
}

bool beProgramBuilderOpenGL::IsInitialized()
{
	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return true;
}

void beProgramBuilderOpenGL::ReleaseProgram()
{
	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return;
}

beKA::beStatus beProgramBuilderOpenGL::GetDeviceTable(std::vector<GDT_GfxCardInfo>& table) const
{
	beKA::beStatus ret = beKA::beStatus_General_FAILED;
	table.clear();

	// Populate the sorted device (card) info table.
	std::vector<GDT_GfxCardInfo> cardList;

	if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SOUTHERNISLAND, cardList))
	{
		table.insert(table.end(), cardList.begin(), cardList.end());
	}

	if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_SEAISLAND, cardList))
	{
		table.insert(table.end(), cardList.begin(), cardList.end());
	}

	if (AMDTDeviceInfoUtils::Instance()->GetAllCardsInHardwareGeneration(GDT_HW_GENERATION_VOLCANICISLAND, cardList))
	{
		table.insert(table.end(), cardList.begin(), cardList.end());
	}

	std::sort(table.begin(), table.end(), beUtils::GfxCardInfoSortPredicate);

	if (!table.empty())
	{
		ret = beKA::beStatus_SUCCESS;
	}

	return ret;
}

bool beProgramBuilderOpenGL::CompileOK(std::string& device)
{
	GT_UNREFERENCED_PARAMETER(device);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return true;
}

beKA::beStatus beProgramBuilderOpenGL::Initialize(const std::string& sDllModule /*= ""*/)
{
	GT_UNREFERENCED_PARAMETER(sDllModule);

	// TODO: remove as part of refactoring.
	// In the executable-oriented architecture, this operation is no longer meaningful.
	return beKA::beStatus_SUCCESS;
}

beKA::beStatus beProgramBuilderOpenGL::Compile(const OpenGLOptions& glOptions, bool& cancelSignal)
{
	GT_UNREFERENCED_PARAMETER(cancelSignal);
	beKA::beStatus ret = beKA::beStatus_General_FAILED;

	// Get VC's path.
	std::string vcPath;
	GetVirtualContextPath(vcPath);

	AMDTDeviceInfoUtils* pDeviceInfo = AMDTDeviceInfoUtils::Instance();
	if (pDeviceInfo != nullptr)
	{
		const char VC_CMD_DELIMITER = ';';

		// Build the command for invoking Virtual Context.
		std::stringstream cmd;

		// ISA.
		cmd << vcPath << " \"" << glOptions.m_isaDisassemblyOutputFiles.m_vertexShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaDisassemblyOutputFiles.m_tessControlShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaDisassemblyOutputFiles.m_tessEvaluationShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaDisassemblyOutputFiles.m_geometryShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaDisassemblyOutputFiles.m_fragmentShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaDisassemblyOutputFiles.m_computeShader.asASCIICharArray() << VC_CMD_DELIMITER;

		// Binaries.
		cmd << glOptions.m_isaBinaryFiles.m_vertexShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaBinaryFiles.m_tessControlShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaBinaryFiles.m_tessEvaluationShader.asASCIICharArray() << VC_CMD_DELIMITER;



		cmd << glOptions.m_isaBinaryFiles.m_geometryShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaBinaryFiles.m_fragmentShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_isaBinaryFiles.m_computeShader.asASCIICharArray() << VC_CMD_DELIMITER;

		// Statistics.
		cmd << glOptions.m_scStatisticsOutputFiles.m_vertexShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_scStatisticsOutputFiles.m_tessControlShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_scStatisticsOutputFiles.m_tessEvaluationShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_scStatisticsOutputFiles.m_geometryShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_scStatisticsOutputFiles.m_fragmentShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_scStatisticsOutputFiles.m_computeShader.asASCIICharArray() << VC_CMD_DELIMITER;

		// Target device info.
		cmd << glOptions.m_chipFamily << VC_CMD_DELIMITER << glOptions.m_chipRevision << VC_CMD_DELIMITER;

		// Input shaders.
		cmd << glOptions.m_pipelineShaders.m_vertexShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_pipelineShaders.m_tessControlShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_pipelineShaders.m_tessEvaluationShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_pipelineShaders.m_geometryShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_pipelineShaders.m_fragmentShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << glOptions.m_pipelineShaders.m_computeShader.asASCIICharArray() << VC_CMD_DELIMITER;
		cmd << "\"";

		// Build the GL program.
		gtString vcOutput;
		bool isLaunchSuccess = osExecAndGrabOutput(cmd.str().c_str(), cancelSignal, vcOutput);

		if (isLaunchSuccess)
		{
			ret = beKA::beStatus_SUCCESS;
		}
	}

	return ret;
}
