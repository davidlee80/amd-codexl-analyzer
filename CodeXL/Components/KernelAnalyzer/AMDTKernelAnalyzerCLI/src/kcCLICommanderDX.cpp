//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcCLICommanderDX.cpp 
/// 
//=====================================================================


// Local.
#include <AMDTKernelAnalyzerCLI/src/kcCLICommanderDX.h>
#include <AMDTKernelAnalyzerCLI/src/kcCliStringConstants.h>
#include <AMDTKernelAnalyzerCLI/src/kcFiles.h>
#include <AMDTKernelAnalyzerCLI/src/kcUtils.h>

// Boost.
#include <boost/algorithm/string/predicate.hpp>

// Backend.
#include <AMDTBackEnd/Include/beProgramBuilderDX.h>
#include <DeviceInfoUtils.h>

// Infra.
#include <AMDTBaseTools/Include/gtAssert.h>

kcCLICommanderDX::kcCLICommanderDX(void)
{
    be = NULL;
}


kcCLICommanderDX::~kcCLICommanderDX(void)
{
    // No need to call DeleteInstance. The base class singleton performs this.
}

/// List the asics as got from device
void kcCLICommanderDX::ListAsics(Config& config, LoggingCallBackFuncP callback)
{
    if (!Init(config, callback))
        return;

    std::stringstream s_Log;
    
    if (! config.m_bVerbose)
    {
        s_Log << "Devices:" << endl;
        string calName;
        for (vector<GDT_GfxCardInfo>::const_iterator it = m_dxDefaultAsicsList.begin(); it != m_dxDefaultAsicsList.end(); ++it)
        {
            calName = string(it->m_szCALName);
            s_Log << "    " << calName << endl;
        }
    }
    else
    {
        // Some headings:
        s_Log << "Devices:" << endl;
        s_Log << "-------------------------------------" << endl;
        s_Log << "Hardware Generation:" << endl;
        s_Log << "    ASIC name" << endl;
        s_Log << "            DeviceID   Marketing Name" << endl;
        s_Log << "-------------------------------------" << endl;

        std::vector<GDT_GfxCardInfo> dxDeviceTable;
        beStatus bRet = be->theOpenDXBuilder()->GetDeviceTable(dxDeviceTable);
        if (bRet == beStatus_SUCCESS)
        {
            GDT_HW_GENERATION gen = GDT_HW_GENERATION_NONE;
            std::string calName;
            for (const GDT_GfxCardInfo& gfxDevice : dxDeviceTable)
            {
                if (gen != gfxDevice.m_generation)
                {
                    gen = gfxDevice.m_generation;
                    std::string sHwGenDisplayName;
                    AMDTDeviceInfoUtils::Instance()->GetHardwareGenerationDisplayName(gen, sHwGenDisplayName);

                    switch (gfxDevice.m_generation)
                    {
                    case GDT_HW_GENERATION_SOUTHERNISLAND:
                        s_Log << sHwGenDisplayName << KA_STR_familyNameSICards ":" << endl;
                        break;
                    case GDT_HW_GENERATION_SEAISLAND:
                        s_Log << sHwGenDisplayName << KA_STR_familyNameCICards ":" << endl;
                        break;
                    case GDT_HW_GENERATION_VOLCANICISLAND:
                        s_Log << sHwGenDisplayName << KA_STR_familyNameVICards ":" << endl;
                        break;
                    default:
                        GT_ASSERT_EX(false, L"Unknown hardware generation.");
                        break;
                    }
                }

                if (calName.compare(gfxDevice.m_szCALName) != 0)
                {
                    calName = std::string(gfxDevice.m_szCALName);
                    s_Log << "    " << calName << endl;
                }

                std::stringstream ss;
                ss << hex << gfxDevice.m_deviceID;
                s_Log << "            " << ss.str() << "       " << string(gfxDevice.m_szMarketingName) << endl;

                }
            }
        else
        {
            s_Log << "Could not generate device table.";
        }
    }
    LogCallBack(s_Log.str());
}

/// list the driver version
void kcCLICommanderDX::Version(Config& config, LoggingCallBackFuncP callback)
{
    if (Init(config, callback))
    {
        std::string catalystVersion;
        bool isOk = be->GetDriverVersionInfo(catalystVersion);
        if(isOk)
        {
            stringstream s_Log;
            s_Log << "Installed Driver Version:" << endl;
            s_Log << catalystVersion.c_str() << endl;
            LogCallBack(s_Log.str());
        }
    }
}


void kcCLICommanderDX::InitRequestedAsicList(const Config& config)
{
    stringstream s_Log;
    
    // Get the default device list.
    if (!config.m_ASICs.empty())
    {
        m_dxDefaultAsicsList.clear();
        std::vector<GDT_GfxCardInfo> dxDeviceTable;
        if (be->theOpenDXBuilder()->GetDeviceTable(dxDeviceTable) == beStatus_SUCCESS)
        {
            for (const std::string& gfxDevice : config.m_ASICs)
            {
                bool isDeviceSupported = false;

                // Both CAL and Marketing names are accepted (case-insensitive).
                for (const GDT_GfxCardInfo& dxDevice : dxDeviceTable)
                {
                    if ((boost::iequals(dxDevice.m_szCALName, gfxDevice)) ||
                        (boost::iequals(dxDevice.m_szMarketingName, gfxDevice)))
                    {
                        isDeviceSupported = true;
                        m_dxDefaultAsicsList.push_back(dxDevice);

                        // We are done.
                        break;
                    }
                }

                if (!isDeviceSupported)
                {
                    // Notify the user.
                    std::stringstream msg;
                    msg << KA_CLI_STR_ERR_D3D_COMPILATION_NOT_SUPPORTED_FOR_DEVICE << gfxDevice << ".\n";
                    LogCallBack(msg.str().c_str());
                }
            }
        }
    }
}

/// Output the ISA representative of the compilation
void kcCLICommanderDX::RunCompileCommands(const Config& config, LoggingCallBackFuncP callback)
{
    bool isInitSuccessful = Init(config, callback);
    if (isInitSuccessful)
    {
        bool bIL = (config.m_ILFile.length() > 0);
        if (bIL)
        {
            callback("Warning: IL code generation for DX is currently not supported. --il command line switch will be ignored.\n");
        }

        bool bISA = false;
        bool bStatistics = false;
		bool bRegisterLiveness = false;
        if (config.m_ISAFile.length() > 0)
        {
            bISA = true;
        }
        if (config.m_AnalysisFile.length() > 0)
        {
            bStatistics = true;
        }
		if (config.m_LiveRegisterAnalysisFile.length() > 0)
		{
			bRegisterLiveness = true;
		}
        vector <AnalysisData> AnalysisDataVec;
        vector <string> DeviceAnalysisDataVec;

        // check for input correctness
        if ((config.m_FXC.length() > 0) && (config.m_SourceLanguage != SourceLanguage_DXasm))
        {
            std::stringstream s_Log;
            s_Log << "DXAsm must be specified when using FXC";
            LogCallBack(s_Log.str());
            return;
        }

        // check flags first
        if ((config.m_Profile.length() == 0) && (config.m_SourceLanguage == SourceLanguage_HLSL))
        {
            std::stringstream s_Log;
            s_Log << "-P Must be specified. Check compiler target: vs_5_0, ps_5_0 etc.";
            LogCallBack(s_Log.str());
            return;
        }

        if ((config.m_SourceLanguage != SourceLanguage_HLSL) && (config.m_SourceLanguage != SourceLanguage_DXasm) && (config.m_SourceLanguage != SourceLanguage_DXasmT))
        {
            std::stringstream s_Log;
            s_Log << "Source language is not supported. Please use ";
            LogCallBack(s_Log.str());
            return;
        }

        // Run FXC if required. It must be first because this is the input for the compilation. we cannot check for success.
        if (config.m_FXC.length() > 0)
        {
            std::string fixedCmd("\"");
            fixedCmd += config.m_FXC;
            fixedCmd += "\"";
            int iRet = ::system(fixedCmd.c_str());
            if (iRet != 0)
            {
                std::stringstream s_Log;
                s_Log << "FXC failed. Please check the arguments and path are correct. If path contains spaces, you need to put it in \\\"\\\" for example\n";
                s_Log << "-f  VsMain1 -s DXAsm -p vs_5_0 c:\\temp\\ClippingBlob.obj  --isa c:\\temp\\dxTest.isa -c tahiti --FXC \"\\\"C:\\Program Files (x86)\\Windows Kits\\8.1\\bin\\x86\\fxc.exe\\\" /E VsMain1 /T vs_5_0  /Fo c:/temp/ClippingBlob.obj c:/temp/Clipping.fx\" ";
                LogCallBack(s_Log.str());
                return;
            }
        }

        // see if the user asked for specific asics
        InitRequestedAsicList(config);

        // for logging purposes we will iterate through the requested ASICs, if input by user
        std::vector<string>::const_iterator asicIter;
        if (!config.m_ASICs.empty())
            asicIter = config.m_ASICs.begin();

        // We need to iterate over the selected devices
        bool bCompileSucces = false;
        for (std::vector<GDT_GfxCardInfo>::iterator devIter = m_dxDefaultAsicsList.begin(); devIter != m_dxDefaultAsicsList.end(); ++devIter)
        {
            // prepare for logging
            string sDevicenametoLog;
            if (!config.m_ASICs.empty() && asicIter != config.m_ASICs.end())
            {
                sDevicenametoLog = *asicIter;
                ++asicIter;
            }
            else
            {
                sDevicenametoLog = devIter->m_szCALName;
            }

            if (Compile(config, *devIter, sDevicenametoLog))
            {
                bCompileSucces = true;
                beStatus backendRet;
                size_t isaSizeInBytes = 0;
                string isail;
                bool rc = false;
                bool isIsaSizeDetected = false;
                bool shouldDetectIsaSize = true;

                if (bISA)
                {
                    backendRet = be->theOpenDXBuilder()->GetDxShaderISAText(devIter->m_szCALName, config.m_Function, config.m_Profile, isail);
                    string fileName = config.m_ISAFile;
                    if (backendRet == beStatus_SUCCESS)
                    {
                        std::stringstream s_Log;
                        rc = KAUtils::WriteTextFile(s_Log, fileName, IsaSuffix, isail, sDevicenametoLog, "");
                        LogCallBack(s_Log.str());

                        // Detect the ISA size.
                        isIsaSizeDetected = be->theOpenDXBuilder()->GetIsaSize(isail, isaSizeInBytes);

                        // If we managed to detect the ISA size, don't do it again.
                        shouldDetectIsaSize = !isIsaSizeDetected;

						if (bRegisterLiveness)
						{
							// Call the kcUtils routine to analyze <generatedFileName> and write
							// the analysis file.s
						}
                    }

                    if ((backendRet == beStatus_SUCCESS) && rc)
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_ISA << sDevicenametoLog << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                        LogCallBack(s_Log.str());
                    }
                    else
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_ISA << sDevicenametoLog << KA_CLI_STR_STATUS_FAILURE << std::endl;
                        LogCallBack(s_Log.str());
                    }
                }

                if (bStatistics)
                {
                    AnalysisData analysis;

                    backendRet = be->theOpenDXBuilder()->GetStatistics(devIter->m_szCALName, config.m_Function, analysis);
                    if (backendRet == beStatus_SUCCESS)
                    {
                        if (shouldDetectIsaSize)
                        {
                            backendRet = be->theOpenDXBuilder()->GetDxShaderISAText(devIter->m_szCALName, config.m_Function, config.m_Profile, isail);
                            if (backendRet == beStatus_SUCCESS)
                            {
                                // Detect the ISA size.
                                isIsaSizeDetected = be->theOpenDXBuilder()->GetIsaSize(isail, isaSizeInBytes);
                            }
                        }

                        if (isIsaSizeDetected)
                        {
                            // assign IsaSize returned above
                            analysis.ISASize = isaSizeInBytes;
                        }
                        else
                        {
                            // assign largest unsigned value, used as warning
                            LogCallBack("Warning: ISA size not available.\n");
                        }

                        // Get WavefrontSize
                        size_t nWavefrontSize = 0;
                        if (be->theOpenDXBuilder()->GetWavefrontSize(devIter->m_szCALName, nWavefrontSize))
                        {
                            analysis.wavefrontSize = nWavefrontSize;
                        }
                        else
                        {
                            LogCallBack("Warning: wavefrontSize size not available.\n");
                        }

                        AnalysisDataVec.push_back(analysis);
                        DeviceAnalysisDataVec.push_back(sDevicenametoLog);

                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_STATISTICS << sDevicenametoLog << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                        LogCallBack(s_Log.str());
                    }
                    else
                    {
                        std::stringstream s_Log;
                        s_Log << KA_CLI_STR_EXTRACTING_STATISTICS << sDevicenametoLog << KA_CLI_STR_STATUS_FAILURE << std::endl;
                        LogCallBack(s_Log.str());
                    }
                }
            }
            std::stringstream s_Log;
            LogCallBack(s_Log.str());
        }

        if ((AnalysisDataVec.size() > 0) && bCompileSucces)
        {
            std::stringstream s_Log;
            WriteAnalysisDataForDX(config, AnalysisDataVec, DeviceAnalysisDataVec, config.m_AnalysisFile, s_Log);
            LogCallBack(s_Log.str());
        }

        // this we should do only once because it is the same to all devices
        if ((config.m_DumpMSIntermediate.size() > 0) && bCompileSucces)
        {
            string sDumpMSIntermediate;
            beStatus beRet = be->theOpenDXBuilder()->GetIntermediateMSBlob(sDumpMSIntermediate);
            if (beRet == beStatus_SUCCESS)
            {
                std::stringstream s_Log;
                if (KAUtils::WriteTextFile(s_Log, config.m_DumpMSIntermediate, "", sDumpMSIntermediate, "", ""))
                {
                    std::stringstream ss;
                    ss << KA_CLI_STR_D3D_ASM_GENERATION_SUCCESS << config.m_DumpMSIntermediate << endl;
                    LogCallBack(ss.str());
                }
                else
                {
                    std::stringstream ss;
                    ss << KA_CLI_STR_D3D_ASM_GENERATION_FAILURE << s_Log.str() << endl;
                    LogCallBack(ss.str());
                }
            }
        }
    }
}

bool kcCLICommanderDX::WriteAnalysisDataForDX(const Config& config, const std::vector<AnalysisData>& AnalysisDataVec,
	const std::vector<string>& DeviceAnalysisDataVec, const std::string& sAnalysisFile, std::stringstream& log)
{
    // Get the separator for CSV list items.
	char csvSeparator = kcUtils::GetCsvSeparator(config);

    // Open output file.
    ofstream output;
    output.open(sAnalysisFile);

    if (! output.is_open())
    {
        log << "Error: Unable to open " << sAnalysisFile << " for write.\n";
    }
    else
    {
        // Write the headers.
        output << kcUtils::GetStatisticsCsvHeaderString(csvSeparator) << std::endl;

        // Write the statistics data.
        if (!AnalysisDataVec.empty())
        {
            std::vector<std::string>::const_iterator iter = DeviceAnalysisDataVec.begin();
            std::vector<AnalysisData>::const_iterator iterAd = AnalysisDataVec.begin();
            for (; iter < DeviceAnalysisDataVec.end(); ++iter, ++iterAd)
            {
                // Device name.
                output << *iter << csvSeparator;

                // Get the Analysis item.
                const AnalysisData& ad = *iterAd;

                // Scratch registers.
                output << ad.maxScratchRegsNeeded << csvSeparator;

                // Work-items per work-group.
                output << ad.numThreadPerGroup << csvSeparator;

                // Wavefront size.
                output << ad.wavefrontSize << csvSeparator;

                // LDS available bytes.
                output << ad.LDSSizeAvailable << csvSeparator;

                // LDS actual bytes.
                output << ad.LDSSizeUsed << csvSeparator;

                // Available SGPRs.
                output << ad.numSGPRsAvailable << csvSeparator;

                // Used SGPRs.
                output << ad.numSGPRsUsed << csvSeparator;

                // Available VGPRs.
                output << ad.numVGPRsAvailable << csvSeparator;

                // Used VGPRs.
                output << ad.numVGPRsUsed << csvSeparator;

                // CL Work-group dimensions (for a unified format, to be revisited).
                output << ad.numThreadPerGroupX << csvSeparator;
                output << ad.numThreadPerGroupY << csvSeparator;
                output << ad.numThreadPerGroupZ << csvSeparator;
                
                // ISA size.
				output << ad.ISASize;

                output << endl;
            }
        }
        output.close();
    }
    return true;
}


/// Output the ISA representative of the compilation
bool kcCLICommanderDX::Compile(const Config& config, GDT_GfxCardInfo& gfxCardInfo, string sDevicenametoLog)
{
    bool bRet = false;
    std::stringstream s_Log;

    /// basically, dx supports offline compilation of dx10/dx11. I will not fail this but will give a warning
    if ((config.m_Profile.find("_3_") != string::npos) || (config.m_Profile.find("_2_") != string::npos) || (config.m_Profile.find("_1_") != string::npos)
        || (config.m_Profile.find("level_9_") != string::npos))
    {
        s_Log << STR_WRN_DX_MIN_SUPPORTED_VERSION << std::endl;
        
        // Notify the user.
        LogCallBack(s_Log.str());

        // Clear the stream.
        s_Log.str("");
    }

    
    //// prepare the options
    beProgramBuilderDX::DXOptions dxOptions;
    dxOptions.m_Entrypoint = config.m_Function;
    dxOptions.m_Target = config.m_Profile;
    dxOptions.m_DXFlags.flagsAsInt = config.m_DXFlags;
    dxOptions.m_bDumpMSIntermediate = (config.m_DumpMSIntermediate.size() > 0 ? true : false);

    // Process config.m_Defines
    // The interface for DX is different here.
    // It is set up for the GUI.
    // The CLI does some work here to translate.
    for (vector<string>::const_iterator it = config.m_Defines.begin();
        it != config.m_Defines.end();
        ++it)
    {
        size_t equal_pos = it->find('=');
        if (equal_pos == string::npos)
        {
            dxOptions.m_defines.push_back(make_pair(*it, string("")));
        }
        else
        {
            dxOptions.m_defines.push_back(make_pair(it->substr(0, equal_pos),
                it->substr(equal_pos + 1, string::npos)));
        }
    }


    // read the source
    string sSource;
    bRet = KAUtils::ReadProgramSource(config.m_InputFile, sSource);
    if (!bRet)
    {
        s_Log << "Error: Unable to read: \'" << config.m_InputFile << "\'." << endl;
    }
    else
    {
        // dx interface like the chip revision and family
        beStatus beRet = be->GetDeviceChipFamilyRevision(gfxCardInfo, dxOptions.m_ChipFamily, dxOptions.m_ChipRevision);
        if (beRet != beStatus_SUCCESS)
        {
            // the use must have got the asics spelled wrong- let him know and continue
            s_Log << "Error: Couldn't find device named: " << sDevicenametoLog << ". Run \'-s HLSL -l --verbose\' to view available devices." << endl;
            bRet = false;
        }
        else
        {
            // Set the source code file name.
            dxOptions.m_FileName = config.m_InputFile;

            // Set the device file name.
            dxOptions.m_deviceName = sDevicenametoLog;

            // Fill the include files path.
            for (const std::string& includePath : config.m_IncludePath)
            {
                dxOptions.m_includeDirectories.push_back(includePath);
            }

            beRet = be->theOpenDXBuilder()->Compile(config.m_SourceLanguage, sSource, dxOptions);
            if (beRet == beStatus_Create_Bolob_FromInput_Failed)
            {
                s_Log << "Error reading DX ASM file. ";
                bRet = false;
            }
            if (beRet != beStatus_SUCCESS)
            {
                s_Log << KA_CLI_STR_COMPILING << sDevicenametoLog << KA_CLI_STR_STATUS_FAILURE << std::endl;
                bRet = false;
            }
            else
            {
                s_Log << KA_CLI_STR_COMPILING << sDevicenametoLog << KA_CLI_STR_STATUS_SUCCESS << std::endl;
                bRet = true;
            }
        }
    }
    LogCallBack(s_Log.str());
    return bRet;
}

bool kcCLICommanderDX::Init(const Config& config, LoggingCallBackFuncP callback)
{
    bool bCont = true;

    m_LogCallback = callback;

    // initialize backend
    be = Backend::Instance();
    if (!be->Initialize(BuiltProgramKind_DX, callback, config.m_DXLocation))
    {
        bCont = false;
    }
    
    if (bCont)// init ASICs list
    {
        std::vector<GDT_GfxCardInfo> dxDeviceTable;
        beStatus bRet = be->theOpenDXBuilder()->GetDeviceTable(dxDeviceTable);
        if (bRet == beStatus_SUCCESS)
        {
            string calName;
            for (vector<GDT_GfxCardInfo>::const_iterator it = dxDeviceTable.begin(); it != dxDeviceTable.end(); ++it)
            {
                if (calName != string(it->m_szCALName))
                {
                    calName = it->m_szCALName;
                    m_dxDefaultAsicsList.push_back(*it);
                }
            }
        }
        else
        {
            bCont = false;
        }
    }
    return bCont;
}

