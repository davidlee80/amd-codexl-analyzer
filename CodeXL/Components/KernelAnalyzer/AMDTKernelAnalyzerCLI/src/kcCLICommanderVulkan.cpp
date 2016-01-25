//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommanderVulkan.cpp 
/// 
//=====================================================================

// C++.
#include <iterator>

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderVulkan.h>
#include <AMDTKernelAnalyzerCLI/src/kcCliStringConstants.h>
#include <AMDTKernelAnalyzerCLI/src/kcUtils.h>
#include <AMDTKernelAnalyzerCLI/src/kcVulkanStatisticsParser.h>

// Backend.
#include <AMDTBackEnd/Include/beProgramBuilderVulkan.h>

kcCLICommanderVulkan::kcCLICommanderVulkan() : m_pVulkanBuilder(new beProgramBuilderVulkan)
{
}

kcCLICommanderVulkan::~kcCLICommanderVulkan()
{
	delete m_pVulkanBuilder;
}

bool kcCLICommanderVulkan::GetSupportedDevices()
{
	if (m_pVulkanBuilder != nullptr)
	{
		std::vector<GDT_GfxCardInfo> availableDevices;

		if (m_supportedDevicesCache.empty())
		{
			beKA::beStatus status = m_pVulkanBuilder->GetDeviceTable(availableDevices);
			if (status == beKA::beStatus_SUCCESS && !availableDevices.empty())
			{
				// Filter out the duplicates.
				for (const GDT_GfxCardInfo& device : availableDevices)
				{
					if (device.m_szCALName != nullptr && strlen(device.m_szCALName) > 1)
					{
						m_supportedDevicesCache.insert(device.m_szCALName);
					}
				}
			}
		}
	}
	return !m_supportedDevicesCache.empty();
}


void kcCLICommanderVulkan::ListAsics(Config& config, LoggingCallBackFunc_t callback)
{
	GT_UNREFERENCED_PARAMETER(config);
	
	// Output message.
	std::stringstream logMsg;

	// Todo: handle the verbose part.
	if (m_supportedDevicesCache.empty())
	{
		if (m_pVulkanBuilder != nullptr)
		{
			bool isDeviceListExtracted = GetSupportedDevices();
			if (!isDeviceListExtracted)
			{
				logMsg << STR_ERR_CANNOT_EXTRACT_SUPPORTED_DEVICE_LIST << std::endl;
			}
		}
	}

	// Print the list of unique device names.
	for (const std::string& device : m_supportedDevicesCache)
	{
		logMsg << device << std::endl;
	}

	// Print the output messages.
	if (callback != nullptr)
	{
		callback(logMsg.str());
	}
}

void kcCLICommanderVulkan::Version(Config& config, LoggingCallBackFunc_t callback)
{
	GT_UNREFERENCED_PARAMETER(config);
	GT_UNREFERENCED_PARAMETER(callback);
	 // TODO: Implement the Vulkan version functionality.
}


void kcCLICommanderVulkan::RunCompileCommands(const Config& config, LoggingCallBackFunc_t callback)
{
	// Output stream.
	std::stringstream logMsg;

	// Input validation.
	bool shouldAbort = false;
	bool isVertexShaderPresent =			(!config.m_VertexShader.empty());
	bool isTessControlShaderPresent =		(!config.m_TessControlShader.empty());
	bool isTessEvaluationShaderPresent =	(!config.m_TessEvaluationShader.empty()); 
	bool isGeometryexShaderPresent =		(!config.m_GeometryShader.empty());
	bool isFragmentShaderPresent =			(!config.m_FragmentShader.empty());
	bool isComputeShaderPresent =			(!config.m_ComputeShader.empty());
	bool isIsaRequired =					(!config.m_ISAFile.empty());
	bool isLiveRegAnalysisRequired =		(!config.m_LiveRegisterAnalysisFile.empty());
	bool isIsaBinary =						(!config.m_BinaryOutputFile.empty());
	bool isIlRequired =						(!config.m_ILFile.empty());
	bool isStatisticsRequired =				(!config.m_AnalysisFile.empty());


	// Fatal error. This should not happen unless we have an allocation problem.
	if (m_pVulkanBuilder == nullptr)
	{
		shouldAbort = true;
		logMsg << STR_ERR_MEMORY_ALLOC_FAILURE << std::endl;
	}

	// Cannot mix compute and non-compute shaders in Vulkan.
	if (isComputeShaderPresent && (isVertexShaderPresent || isTessControlShaderPresent ||
		isTessEvaluationShaderPresent || isGeometryexShaderPresent || isFragmentShaderPresent || isComputeShaderPresent))
	{
		logMsg << STR_ERR_RENDER_COMPUTE_MIX << std::endl;
		shouldAbort = true;
	}
	
	// Options to be passed to the backend.
	VulkanOptions vulkanOptions;

	// Validate the input shaders.
	if (!shouldAbort && isVertexShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_VERTEX_SHADER, config.m_VertexShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_vertexShader << config.m_VertexShader.c_str();
	}
	if (!shouldAbort && isTessControlShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_TESS_CTRL_SHADER, config.m_TessControlShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_tessControlShader << config.m_TessControlShader.c_str();
	}
	if (!shouldAbort && isTessEvaluationShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_TESS_EVAL_SHADER, config.m_TessEvaluationShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_tessEvaluationShader << config.m_TessEvaluationShader.c_str();
	}
	if (!shouldAbort && isGeometryexShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_GEOMETRY_SHADER, config.m_GeometryShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_geometryShader << config.m_GeometryShader.c_str();
	}
	if (!shouldAbort && isFragmentShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_FRAGMENT_SHADER, config.m_FragmentShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_fragmentShader << config.m_FragmentShader.c_str();
	}
	if (!shouldAbort && isComputeShaderPresent)
	{
		shouldAbort = !kcUtils::ValidateShaderFileName(KA_CLI_STR_COMPUTE_SHADER, config.m_ComputeShader, logMsg);
		vulkanOptions.m_pipelineShaders.m_computeShader << config.m_ComputeShader.c_str();
	}

	// Validate the output directories.
	if (!shouldAbort && isIsaRequired)
	{
		shouldAbort = !kcUtils::ValidateShaderOutputDir(config.m_ISAFile, logMsg);
	}
	if (!shouldAbort && isLiveRegAnalysisRequired)
	{
		shouldAbort = !kcUtils::ValidateShaderOutputDir(config.m_LiveRegisterAnalysisFile, logMsg);
	}
	if (!shouldAbort && isIlRequired)
	{
		shouldAbort = !kcUtils::ValidateShaderOutputDir(config.m_ILFile, logMsg);
	}
	if (!shouldAbort && isStatisticsRequired)
	{
		shouldAbort = !kcUtils::ValidateShaderOutputDir(config.m_AnalysisFile, logMsg);
	}
	if (!shouldAbort && isIsaBinary)
	{
		shouldAbort = !kcUtils::ValidateShaderOutputDir(config.m_BinaryOutputFile, logMsg);
	}

	if (!shouldAbort)
	{
		// Set the log callback for the backend.
		m_pVulkanBuilder->SetLog(callback);

		// If the user did not specify any device, we should use all supported devices.
		std::vector<std::string> targetDecives;
		bool shouldUseAlldevices = config.m_ASICs.empty();

		if (shouldUseAlldevices && m_supportedDevicesCache.empty())
		{
			// We need to populate the list of supported devices.
			bool isDeviceListExtracted = GetSupportedDevices();
			if (!isDeviceListExtracted)
			{
				std::stringstream errMsg;
				errMsg << STR_ERR_CANNOT_EXTRACT_SUPPORTED_DEVICE_LIST << std::endl;
				shouldAbort = true;
			}
		}

		if (!shouldAbort)
		{
			if (!shouldUseAlldevices)
			{
				// If the user specified a device, go with the user's choice.
				targetDecives = config.m_ASICs;
			}
			else
			{
				// Otherwise, use the cached list of all supported devices.
				std::copy(m_supportedDevicesCache.begin(), m_supportedDevicesCache.end(), std::back_inserter(targetDecives));
			}

			for (const std::string& device : targetDecives)
			{
				// Generate the output message.
				logMsg << KA_CLI_STR_COMPILING << device;

				// Set the target device for the backend.
				vulkanOptions.m_targetDeviceName = device;

				// Adjust the output file names to the device and shader type.
				if (isIsaRequired)
				{
					vulkanOptions.m_isAmdIsaDisassemblyRequired = true;
					kcUtils::AdjustRenderingPipelineOutputFileNames(config.m_ISAFile, device, vulkanOptions.m_isaDisassemblyOutputFiles);
				}
				if (isLiveRegAnalysisRequired)
				{
					vulkanOptions.m_isLiveRegisterAnalysisRequired = true;
					kcUtils::AdjustRenderingPipelineOutputFileNames(config.m_LiveRegisterAnalysisFile, device, vulkanOptions.m_liveRegisterAnalysisOutputFiles);
				}
				if (isIlRequired)
				{
					vulkanOptions.m_isAmdPalIlDisassemblyRequired = true;
					kcUtils::AdjustRenderingPipelineOutputFileNames(config.m_ILFile, device, vulkanOptions.m_pailIlDisassemblyOutputFiles);
				}
				if (isStatisticsRequired)
				{
					vulkanOptions.m_isScStatsRequired = true;
					kcUtils::AdjustRenderingPipelineOutputFileNames(config.m_AnalysisFile, device, vulkanOptions.m_scStatisticsOutputFiles);
				}
				if (isIsaBinary)
				{
					vulkanOptions.m_isAmdIsaBinariesRequired = true;
					kcUtils::AdjustRenderingPipelineOutputFileNames(config.m_BinaryOutputFile, device, vulkanOptions.m_isaBinaryFiles);
				}

				// A handle for canceling the build. Currently not in use.
				bool shouldCancel = false;

				// Compile.
				beKA::beStatus compilationStatus = m_pVulkanBuilder->Compile(vulkanOptions, shouldCancel);
				if (compilationStatus == beStatus_SUCCESS)
				{
					logMsg << KA_CLI_STR_STATUS_SUCCESS << std::endl;

					// Parse the statistics file if required.
					if (isStatisticsRequired)
					{
						beKA::AnalysisData statistics;
						kcVulkanStatisticsParser statsParser;
						if (isVertexShaderPresent)
						{
							kcUtils::ReplaceStatisticsFile(vulkanOptions.m_scStatisticsOutputFiles.m_vertexShader, config, device, statsParser, callback);
						}		
						if (isTessControlShaderPresent)
						{
							kcUtils::ReplaceStatisticsFile(vulkanOptions.m_scStatisticsOutputFiles.m_tessControlShader, config, device, statsParser, callback);
						}
						if (isTessControlShaderPresent)
						{
							kcUtils::ReplaceStatisticsFile(vulkanOptions.m_scStatisticsOutputFiles.m_tessEvaluationShader, config, device, statsParser, callback);
						}
						if (isFragmentShaderPresent)
						{
							kcUtils::ReplaceStatisticsFile(vulkanOptions.m_scStatisticsOutputFiles.m_fragmentShader, config, device, statsParser, callback);
						}
						if (isComputeShaderPresent)
						{
							kcUtils::ReplaceStatisticsFile(vulkanOptions.m_scStatisticsOutputFiles.m_computeShader, config, device, statsParser, callback);
						}
					}

					// Perform live register analysis if required.
					if (isLiveRegAnalysisRequired)
					{
						if (isVertexShaderPresent)
						{
							kcUtils::PerformLiveRegisterAnalysis(vulkanOptions.m_isaDisassemblyOutputFiles.m_vertexShader, 
								vulkanOptions.m_liveRegisterAnalysisOutputFiles.m_vertexShader, callback);
						}
						if (isTessControlShaderPresent)
						{
							kcUtils::PerformLiveRegisterAnalysis(vulkanOptions.m_isaDisassemblyOutputFiles.m_tessControlShader,
								vulkanOptions.m_liveRegisterAnalysisOutputFiles.m_tessControlShader, callback);
						}
						if (isTessControlShaderPresent)
						{
							kcUtils::PerformLiveRegisterAnalysis(vulkanOptions.m_isaDisassemblyOutputFiles.m_tessEvaluationShader,
								vulkanOptions.m_liveRegisterAnalysisOutputFiles.m_tessEvaluationShader, callback);
						}
						if (isFragmentShaderPresent)
						{
							kcUtils::PerformLiveRegisterAnalysis(vulkanOptions.m_isaDisassemblyOutputFiles.m_fragmentShader,
								vulkanOptions.m_liveRegisterAnalysisOutputFiles.m_fragmentShader, callback);
						}
						if (isComputeShaderPresent)
						{
							kcUtils::PerformLiveRegisterAnalysis(vulkanOptions.m_isaDisassemblyOutputFiles.m_computeShader,
								vulkanOptions.m_liveRegisterAnalysisOutputFiles.m_computeShader, callback);
						}
					}
				}
				else if (compilationStatus == beStatus_VulkanAmdspvCompilationFailure)
				{
					logMsg << KA_CLI_STR_STATUS_FAILURE << std::endl;
				}
				else if (compilationStatus == beStatus_VulkanAmdspvLaunchFailure)
				{
					logMsg << STR_ERR_CANNOT_INVOKE_COMPILER << std::endl;
				}

				// Print the message for the current device.
				callback(logMsg.str());

				// Clear the output stream for the next iteration.
				logMsg.str("");
			}
		}
	}
	else
	{
		logMsg << KA_CLI_STR_ABORTING << std::endl;
	}

	// Print the output message.
	if (callback != nullptr)
	{
		callback(logMsg.str());
	}
}
