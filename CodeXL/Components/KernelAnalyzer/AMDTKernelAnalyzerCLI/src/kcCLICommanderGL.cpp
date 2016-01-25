//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommanderGL.cpp 
/// 
//=====================================================================
#include <AMDTBackEnd/Include/GL/glew.h>
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderGL.h>

// Infra.
#include <AMDTBaseTools/Include/gtDefinitions.h>
#include <AMDTBaseTools/Include/gtAssert.h>

// Local.
#include <AMDTKernelAnalyzerCLI/src/kcCliStringConstants.h>
#include <AMDTKernelAnalyzerCLI/src/kcFiles.h>
#include <AMDTKernelAnalyzerCLI/src/kcUtils.h>

// Backend.
#include <AMDTBackEnd/Include/beProgramBuilderGL.h>

// Other.
#include <boost/algorithm/string/predicate.hpp>
#include <DeviceInfoUtils.h>

const std::string STR_GL_VERTEX_SHADER_NAME      = "Vertex";
const std::string STR_GL_FRAGMENT_SHADER_NAME    = "Fragment";
const std::string STR_GL_GEOMETRY_SHADER_NAME    = "Geometry";
const std::string STR_GL_COMPUTE_SHADER_NAME     = "Compute";
const std::string STR_GL_TESS_CTRL_SHADER_NAME   = "TessControl";
const std::string STR_GL_TESS_EVAL_SHADER_NAME   = "TessEval";

kcCLICommanderGL::kcCLICommanderGL() : m_pBackend(nullptr)
{

}

kcCLICommanderGL::~kcCLICommanderGL() 
{
    // No need to call DeleteInstance. The base class singleton performs this.
}

bool kcCLICommanderGL::Init(const Config& config, LoggingCallBackFunc_t callback)
{
    GT_UNREFERENCED_PARAMETER(&config);

    bool ret = false;

    // Set the callback function.
    m_LogCallback = callback;

    // Initialize the backend.
    m_pBackend = Backend::Instance();
    if (m_pBackend != nullptr && m_pBackend->Initialize(BuiltProgramKind_OpenGL, callback, ""))
    {
        // Initialize the device list.
        std::vector<GDT_GfxCardInfo> glDeviceTable;
        beStatus bRet = m_pBackend->theOpenGLBuilder()->GetDeviceTable(glDeviceTable);
        if (bRet == beStatus_SUCCESS)
        {
            std::string calName;
            for (const GDT_GfxCardInfo& glDevice : glDeviceTable)
            {
                if (calName.compare(glDevice.m_szCALName) != 0)
                {
                    calName = glDevice.m_szCALName;
                    m_glDefaultAsicsList.push_back(glDevice);
                }
            }

            // Verify that the GL runtime version is at least the minimum supported version.
            const beProgramBuilderGL* pBuilder = m_pBackend->theOpenGLBuilder();
            if (pBuilder != nullptr)
            {
                ret = true;
                std::string supportedGlVersion;
                bool isVersionExtracted = pBuilder->GetOpenGlVersion(supportedGlVersion);

                // An empty version string means a non-release driver.
                bool isReleaseVersion = !supportedGlVersion.empty();

                if (isReleaseVersion && isVersionExtracted)
                {
                    const char* MIN_GL_RT_VERSION = "4.5.13";
                    if (supportedGlVersion.compare(0, strlen(MIN_GL_RT_VERSION), MIN_GL_RT_VERSION) < 0)
                    {
                        m_LogCallback(STR_ERR_UNSUPPORTED_GL_RT_VERSION);
                        ret = false;
                    }
                }
            }
        }
    }
    return ret;
}

 
// List the devices.
void kcCLICommanderGL::ListAsics(Config& config, LoggingCallBackFunc_t callback)
{
    bool isInitSucceeded = Init(config, callback);
    if (isInitSucceeded)
    {
        std::stringstream logStream;

        if (!config.m_bVerbose)
        {
            logStream << "Devices:" << endl;
            std::string calName;
            for (const GDT_GfxCardInfo& glDevice : m_glDefaultAsicsList)
            {
                calName = string(glDevice.m_szCALName);
                logStream << "    " << calName << endl;
            }
        }
        else
        {
            GT_IF_WITH_ASSERT(m_pBackend != nullptr)
            {
                // Some headings:
                logStream << "Devices:" << endl;
                logStream << "-------------------------------------" << endl;
                logStream << "Hardware Generation:" << endl;
                logStream << "    ASIC name" << endl;
                logStream << "            DeviceID   Marketing Name" << endl;
                logStream << "-------------------------------------" << endl;

                std::vector<GDT_GfxCardInfo> glDeviceTable;
                beStatus bRet = m_pBackend->theOpenGLBuilder()->GetDeviceTable(glDeviceTable);
                if (bRet == beStatus_SUCCESS)
                {
                    GDT_HW_GENERATION gen = GDT_HW_GENERATION_NONE;
                    string calName;
                    for (const GDT_GfxCardInfo& glDevice : glDeviceTable)
                    {
                        if (gen != glDevice.m_generation)
                        {
                            gen = glDevice.m_generation;
                            std::string sHwGenDisplayName;
                            AMDTDeviceInfoUtils::Instance()->GetHardwareGenerationDisplayName(gen, sHwGenDisplayName);

                            switch (glDevice.m_generation)
                            {
                            case GDT_HW_GENERATION_SOUTHERNISLAND:
                                logStream << sHwGenDisplayName << KA_STR_familyNameSICards ":" << endl;
                                break;
                            case GDT_HW_GENERATION_SEAISLAND:
                                logStream << sHwGenDisplayName << KA_STR_familyNameCICards ":" << endl;
                                break;
                            case GDT_HW_GENERATION_VOLCANICISLAND:
                                logStream << sHwGenDisplayName << KA_STR_familyNameVICards ":" << endl;
                                break;
                            default:
                                GT_ASSERT_EX(false, L"Unknown hardware generation.");
                                break;
                            }
                        }

                        if (calName != string(glDevice.m_szCALName))
                        {
                            calName = string(glDevice.m_szCALName);
                            logStream << "    " << calName << endl;
                        }
                        std::stringstream deviceIdStream;
                        deviceIdStream << hex << glDevice.m_deviceID;
                        logStream << "            " << deviceIdStream.str() << "       " << string(glDevice.m_szMarketingName) << endl;

                    }
                }
                else
                {
                    logStream << "Could not generate device table.";
                }
            }
        }
        LogCallBack(logStream.str());
    }
}

/// list the driver version
void kcCLICommanderGL::Version(Config& config, LoggingCallBackFunc_t callback)
{
    GT_UNREFERENCED_PARAMETER(config);
    bool isInitialized = Init(config, callback);
    if (isInitialized)
    {
        bool isVersionExtracted = false;

        if (m_pBackend != nullptr)
        {
            beProgramBuilderGL* pBuilder = m_pBackend->theOpenGLBuilder();
            if (pBuilder != nullptr)
            {
                std::string supportedGlVersion;
                isVersionExtracted = pBuilder->GetOpenGlVersion(supportedGlVersion);
                isVersionExtracted = isVersionExtracted && !supportedGlVersion.empty();
                if (isVersionExtracted)
                {
                    std::stringstream logStream;
                    logStream << supportedGlVersion << std::endl;
                    callback(logStream.str());
                }
            }
        }

        if (!isVersionExtracted)
        {
            callback(STR_ERR_OPENGL_VERSION_EXTRACTION_FAILURE);
        }
    }
    else
    {
        callback(STR_ERR_INITIALIZATION_FAILURE);
    }
}

/// Output multiple commands for all commands that requires compilation: GetBinary, GetIL, GetISA, GetAnlysis, GetMetadata, GetDebugIL,ListKernels
void kcCLICommanderGL::RunCompileCommands(const Config& config, LoggingCallBackFunc_t callback)
{
    bool isInitSucceeded = Init(config, callback);
    if (isInitSucceeded)
    {
        beKA::beStatus beRc = beStatus_General_FAILED;
        bool isOk = false;

        // what do we want to do?
        bool isIsaRequired = false;
		bool isLiveRegisterAnalysisRequired = false;
        bool isIlRequired = false;
        bool isAnalysisRequired = false;

        if (config.m_ISAFile.length() > 0)
        {
            isIsaRequired = true;
        }
        if (config.m_ILFile.length() > 0)
        {
            isIlRequired = true;
        }
        if (config.m_AnalysisFile.length() > 0)
        {
            isAnalysisRequired = true;
        }
		if (config.m_LiveRegisterAnalysisFile.length() > 0)
		{
			isLiveRegisterAnalysisRequired = true;
		}

         // Check if the user asked for specific devices.
        InitRequestedAsicList(config);

        // For logging purposes we will iterate through the requested ASICs, if input by user.
        std::vector<string>::const_iterator asicIter;
        if (!config.m_ASICs.empty())
        {
            asicIter = config.m_ASICs.begin();
        }


        // A container to hold the analysis data.
        std::map<std::string, beKA::AnalysisData> devicesStatisticsData;

        // We need to iterate over the selected devices.
        for (const GDT_GfxCardInfo& gfxCard : m_glDefaultAsicsList)
        {
            // prepare for logging
            std::string currDeviceName;
            if (!config.m_ASICs.empty())
            {
                currDeviceName = *asicIter;
                ++asicIter;
            }
            else
            {
                currDeviceName = gfxCard.m_szCALName;
            }

            if (Compile(config, gfxCard, currDeviceName))
            {
                std::string isaBuffer;
                if (isIsaRequired)
                {
                    beRc = m_pBackend->theOpenGLBuilder()->GetKernelISAText(currDeviceName, "", isaBuffer);
                    if (beRc == beKA::beStatus_SUCCESS)
                    {
                        std::stringstream s_Log;
                        isOk = KAUtils::WriteTextFile(s_Log, config.m_ISAFile, IsaSuffix, isaBuffer, currDeviceName, "");
                        LogCallBack(s_Log.str());

						if (isOk && isLiveRegisterAnalysisRequired)
						{
							// Call the kcUtils routine to analyze <generatedFileName> and write
							// the analysis file.
						}
                    }

                    // Inform the user.
                    if ((beRc == beStatus_SUCCESS) && (isOk))
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_ISA << currDeviceName << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                        LogCallBack(s_Log.str());
                    }
                    else
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_ISA << currDeviceName << KA_CLI_STR_STATUS_FAILURE << std::endl;
                        LogCallBack(s_Log.str());
                    }

                }
                if (isIlRequired)
                {
                    std::string ilBuffer;
                    beRc = m_pBackend->theOpenGLBuilder()->GetKernelILText(gfxCard.m_szCALName, config.m_Function, ilBuffer);
                    if (beRc == beStatus_SUCCESS)
                    {
                        std::stringstream s_Log;
                        isOk = KAUtils::WriteTextFile(s_Log, config.m_ILFile, IsaSuffix, ilBuffer, currDeviceName, "");
                        LogCallBack(s_Log.str());
                    }

                    // Inform the user.
                    if ((beRc == beStatus_SUCCESS) && (beRc))
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_AMDIL << currDeviceName << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                        LogCallBack(s_Log.str());
                    }
                    else
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_AMDIL << currDeviceName << KA_CLI_STR_STATUS_FAILURE << std::endl;
                        LogCallBack(s_Log.str());
                    }
                }
                beKA::AnalysisData analysis;
                if (isAnalysisRequired)
                {
                    std::string currDeviceNameInnerInner = gfxCard.m_szCALName;

                    // Get the statistics.
                    beRc = m_pBackend->theOpenGLBuilder()->GetStatistics(currDeviceNameInnerInner, config.m_Function, analysis);

                    if (beRc == beStatus_SUCCESS)
                    {
                        // Add the analysis data to our container.
                        devicesStatisticsData[currDeviceNameInnerInner] = analysis;
                    }

                    // Inform the user.
                    if (beRc == beStatus_SUCCESS)
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_STATISTICS << currDeviceNameInnerInner << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                        LogCallBack(s_Log.str());
                    }
                    else
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_STATISTICS << currDeviceNameInnerInner << KA_CLI_STR_STATUS_FAILURE << std::endl;
                        LogCallBack(s_Log.str());
                    }
                }
            }
        }

        // Write down the statistics file if required.
        if (!devicesStatisticsData.empty())
        {
            CreateStatisticsFile(config, devicesStatisticsData);
        }
    }
}

void kcCLICommanderGL::InitRequestedAsicList(const Config& config)
{
    // Get the default device list.
    if (!config.m_ASICs.empty())
    {
        m_glDefaultAsicsList.clear();
        std::vector<GDT_GfxCardInfo> dxDeviceTable;
        if (m_pBackend->theOpenGLBuilder()->GetDeviceTable(dxDeviceTable) == beStatus_SUCCESS)
        {
            bool isSupportedDevice = false;

            // Both CAL and Marketing names are accepted (case-insensitive).
            for (const std::string& deviceName : config.m_ASICs)
            {
                isSupportedDevice = false;

                for (const GDT_GfxCardInfo& gfxCard : dxDeviceTable)
                {
                    if ((boost::iequals(gfxCard.m_szCALName, deviceName)) ||
                        (boost::iequals(gfxCard.m_szMarketingName, deviceName)))
                    {
                        m_glDefaultAsicsList.push_back(gfxCard);

                        // We found it.
                        isSupportedDevice = true;
                        break;
                    }
                }

                if (!isSupportedDevice)
                {
                    std::stringstream logStream;
                    logStream << KA_CLI_STR_ERR_GLSL_COMPILATION_NOT_SUPPORTED_A << deviceName <<
                        KA_CLI_STR_ERR_GLSL_COMPILATION_NOT_SUPPORTED_B << std::endl;
                    LogCallBack(logStream.str());
                }
            }
        }
    }
}


bool kcCLICommanderGL::Compile(const Config& config, const GDT_GfxCardInfo& gfxCardInfo, const std::string& deviceNameToLog)
{
    // This stream will be use to inform the upper layers about our progress.
    std::stringstream logStream;

    // Read the source code.
    std::string sourceCode;
    bool ret = KAUtils::ReadProgramSource(config.m_InputFile, sourceCode);

    if (!ret)
    {
        logStream << "Error: Unable to read: \'" << config.m_InputFile << "\'." << endl;
    }
    else
    {
        // Prepare the options.
        beProgramBuilderGL::GLOptions glOptions;

        // Get the chip info.
        beStatus beRet = m_pBackend->GetDeviceChipFamilyRevision(gfxCardInfo, glOptions.m_ChipFamily, glOptions.m_ChipRevision);
        if (beRet != beStatus_SUCCESS)
        {
            // The user must have spelled the devices inaccurately - let him know and continue.
            logStream << "Error: Couldn't find device named: " << deviceNameToLog << ". Run \'-s HLSL -l --verbose\' to view available devices." << endl;
            ret = false;
        }
        else 
        {
            // Set the target profile.
            if (boost::iequals(config.m_Profile, STR_GL_GEOMETRY_SHADER_NAME))
            {
                glOptions.m_Profile = GL_GEOMETRY_SHADER;
            }
            else if (boost::iequals(config.m_Profile, STR_GL_FRAGMENT_SHADER_NAME))
            {
                glOptions.m_Profile = GL_FRAGMENT_SHADER;
            }
            else if (boost::iequals(config.m_Profile, STR_GL_VERTEX_SHADER_NAME))
            {
                glOptions.m_Profile = GL_VERTEX_SHADER;
            }
            else if (boost::iequals(config.m_Profile, STR_GL_COMPUTE_SHADER_NAME))
            {
                glOptions.m_Profile = GL_COMPUTE_SHADER;
            }
            else if (boost::iequals(config.m_Profile, STR_GL_TESS_CTRL_SHADER_NAME))
            {
                glOptions.m_Profile = GL_TESS_CONTROL_SHADER;
            }
            else if (boost::iequals(config.m_Profile, STR_GL_TESS_EVAL_SHADER_NAME))
            {
                glOptions.m_Profile = GL_TESS_EVALUATION_SHADER;
            }
            else
            {
                logStream << STR_ERR_INVALID_GLSL_SHADER_TYPE << std::endl;
                ret = false;
            }

            if (ret)
            {
                beKA::beStatus beStat = m_pBackend->theOpenGLBuilder()->Compile(sourceCode, glOptions, &config.m_InputFile);
                if (beStat == beStatus_SUCCESS)
                {
                    logStream << KA_CLI_STR_COMPILING << deviceNameToLog << KA_CLI_STR_STATUS_SUCCESS << endl;
                    ret = true;
                }
                else
                {
                    logStream << KA_CLI_STR_COMPILING << deviceNameToLog << KA_CLI_STR_STATUS_FAILURE << endl;
                    ret = false;
                }
            }
         }
    }

    // Inform the user.
    LogCallBack(logStream.str());

    return ret;
}

// *******************************************************************************************************************
// Note: this function should be relocated to a utilities class which will be shared to all kcCLICommanderXXX objects.
// *******************************************************************************************************************
void kcCLICommanderGL::CreateStatisticsFile( const Config& config, const std::map<std::string, beKA::AnalysisData>& analysisData )
{
    // Get the separator for CSV list items.
	char csvSeparator = kcUtils::GetCsvSeparator(config);

    // Open output file.
    std::ofstream output;
    output.open(config.m_AnalysisFile.c_str());

    if (!output.is_open())
    {
        std::stringstream s_Log;
        s_Log << "Error: Unable to open " << config.m_AnalysisFile << " for write.\n";
        LogCallBack(s_Log.str());
    }
    else
    {
        // Write the headers.
        output << kcUtils::GetStatisticsCsvHeaderString(csvSeparator) << std::endl;

        for (const auto& deviceStatsPair : analysisData)
        {
            // Write a line of CSV.
            const AnalysisData& analysis = deviceStatsPair.second;
            output << deviceStatsPair.first << csvSeparator;
            output << analysis.maxScratchRegsNeeded << csvSeparator;
            output << analysis.numThreadPerGroup << csvSeparator;
            output << analysis.wavefrontSize << csvSeparator;
            output << analysis.LDSSizeAvailable << csvSeparator;
            output << analysis.LDSSizeUsed << csvSeparator;
            
            // It seems like this info (available SGPRs) is not available for GL.
            output << "n/a" << csvSeparator;

            output << analysis.numSGPRsUsed << csvSeparator;

            // It seems like this info (available VGPRs) is not available for GL.
            output << "n/a" << csvSeparator;

            output << analysis.numVGPRsUsed << csvSeparator;
            output << analysis.numThreadPerGroupX << csvSeparator;
            output << analysis.numThreadPerGroupY << csvSeparator;
            output << analysis.numThreadPerGroupZ << csvSeparator;
			output << analysis.ISASize;
            output << endl;
        }
    }
}
