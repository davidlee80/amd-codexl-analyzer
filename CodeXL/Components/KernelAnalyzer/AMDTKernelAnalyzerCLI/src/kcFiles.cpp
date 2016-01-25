//=====================================================================
// Copyright 2012-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcFiles.cpp 
/// \brief   File read/write/&c. utilities.
/// 
//=====================================================================
#include <AMDTKernelAnalyzerCLI/src/kcFiles.h>

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-variable"
#include <dirent.h>
#endif
#include <boost/filesystem.hpp>

// Infra.
#include <AMDTBaseTools/Include/gtString.h>
#include <AMDTOSWrappers/Include/osFilePath.h>
#include <AMDTOSWrappers/Include/osDirectory.h>

using namespace std;

namespace KAUtils
{

bool
ReadProgramSource(
    const string& inputFile,
    string& programSource)
{
    ifstream input;

#ifndef _WIN32
    // test if the input file is a directory.
    // On some Linux machine ifstream open will be valid if it is a directory
    // but will not allow to read data which will cause a crash when trying to read the data
    DIR *pDir;

    pDir = opendir (inputFile.c_str());

    if (pDir != NULL)
    {
        (void) closedir (pDir);
        return false;
    }
#endif

    // Open (at e)nd of file.  We want the length below.
    // Open binary because it's faster & other bits of code deal OK with CRLF, LF etc.
    input.open(inputFile.c_str(), ios::ate | ios::binary);

    if (!input)
    {
        return false;
    }

    ifstream::pos_type fileSize = 0;

    fileSize = input.tellg();

    if (fileSize == static_cast<ifstream::pos_type>(0))
    {
        input.close();
        return false;
    }

    input.seekg(0, ios::beg);

    programSource.resize(size_t(fileSize));
    input.read(&programSource[0], fileSize);

    input.close();
    return true;
}
string
DecoratePath(
    const string& stem,
    const string& device,
    const string& suffix, 
    const string& kernelName)
{
    boost::filesystem::path path(stem);
    boost::filesystem::path decoratedPath(path.parent_path());

    osFilePath targetPath;
    if (!kernelName.empty())
    {
        // Create a folder for the kernel.
        gtString targetFolder;
        targetFolder << decoratedPath.c_str();
        targetPath.setFileDirectory(targetFolder);

        gtString kernelFolderName;
        kernelFolderName << kernelName.c_str();
        targetPath.appendSubDirectory(kernelFolderName);

        if (!targetPath.exists())
        {
            osDirectory targetDir;
            if (targetPath.getFileDirectory(targetDir))
            {
                assert(targetDir.create());
            }
        }
    }
    else
    {
        gtString tmpDecoratedPath;
        tmpDecoratedPath << decoratedPath.c_str();
        targetPath.setFileDirectory(tmpDecoratedPath);
    }

    gtString fixedFileName;
    gtString fixedFileExtension;

    // Default to .bin as an extension.
    if (suffix.empty())
    {
        fixedFileName << stem.c_str();
        targetPath.setFullPathFromString(fixedFileName);
    }
    else if (path.extension().empty())
    {
        fixedFileName << device.c_str() << "_" << path.stem().string().c_str() << suffix.c_str();
        targetPath.setFileName(fixedFileName);
    }
    else
    {
        fixedFileName << device.c_str() << "_" << path.stem().string().c_str() << path.extension().string().c_str();
        targetPath.setFileName(fixedFileName);
    }

    
    //targetPath.setFileExtension(fixedFileExtension);
    return targetPath.asString().asASCIICharArray();
}

bool
WriteBinaryFile(
    ostream& log,
    const string& outFile,
    bool isPathDecorationRequired,
    const vector<char>& binary,
    const string& deviceName)
{
    string decoratedPath;
    if (isPathDecorationRequired)
    {
        string sNormelizeDeviceName;
        kaNormelizeDeviceName(deviceName, sNormelizeDeviceName);
        decoratedPath = DecoratePath(outFile, sNormelizeDeviceName, "bin", "");
    }
    else
    {
        decoratedPath = outFile;
    }

    ofstream output;
    output.open(decoratedPath.c_str(), ios::binary);

    if (output.is_open())
    {
        output.write(&binary[0], binary.size());

        if (output.fail())
        {
            output.close();

            log << "Error: Unable to write " << decoratedPath << " for write.\n";
            return false;
        }

        output.close();
    }
    else
    {
        log << "Error: Unable to open " << decoratedPath << " for write.\n";
        return false;
    }

    return true;
}

bool WriteTextFile(
    std::ostream& log,
    const std::string& stem,
    const std::string& suffix,
    const std::string& text,
    const std::string& deviceName, const std::string& kernelName)
{
	string sNormelizeDeviceName;
	kaNormelizeDeviceName(deviceName, sNormelizeDeviceName);
    string decoratedPath = DecoratePath(stem, sNormelizeDeviceName, suffix, kernelName);
    ofstream output;
    output.open(decoratedPath.c_str());

    if (output.is_open())
    {
        output.write(text.c_str(), text.size());

        if (output.fail())
        {
            output.close();

            log << "Error: Unable to write " << decoratedPath << " for write.\n";
            return false;
        }

        output.close();
    }
    else
    {
        log << "Error: Unable to open " << decoratedPath << " for write.\n";
        return false;
    }

    return true;
}

void kaNormelizeDeviceName(const std::string& sOrigin, std::string& sDest)
{
	// go over the name and replace funny characters with _ because we later make it as a file name
	sDest = sOrigin;
	std::size_t found = sDest.find("/");

	for (; found != std::string::npos; found = sDest.find("/"))
	{
		sDest.replace(found, 1, "_");
	}

	found = sDest.find(":");

	for (; found != std::string::npos; found = sDest.find(":"))
	{
		sDest.replace(found, 1, "_");
	}
}

}; // namespace
