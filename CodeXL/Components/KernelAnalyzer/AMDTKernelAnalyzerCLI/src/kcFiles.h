//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file kcFiles.h 
/// \brief   File read/write/&c. utilities.
/// 
//=====================================================================

//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/CodeXL/Components/KernelAnalyzer/AMDTKernelAnalyzerCLI/src/kcFiles.h#1 $
// Last checkin:   $DateTime: 2015/12/22 04:40:14 $
// Last edited by: $Author: abenmos $
// Change list:    $Change: 553192 $
//=====================================================================


#ifndef KAUtils_Files_H
#define KAUtils_Files_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace KAUtils
{


/// read named file into a string.
/// \param[in]  inputFile     name of file.
/// \param[out] programSource content of file.
/// \returns                  success.
bool
ReadProgramSource(
    const std::string& inputFile,
    std::string& programSource);

/// Create a path by inserting -device after the root name and appending suffix, if one is not already present.
/// \param stem   The [path/]root[.suffix] to begin with.
/// \param device The name of device to insert.
/// \param suffix A suffix to append if a suffix is not already present.
std::string DecoratePath(
    const std::string& stem,
    const std::string& device, const std::string& kernelName,
    const std::string& suffix);

/// Write a binary file.
/// \param      log          a stream for diagnostic output.
/// \param[in]  outFile      a stem to use for the file name.
/// \param[in]  isPathDecorationRequired   if true then the device name will be added to the output file name.
/// \param[in]  binary       the bytes to write.
/// \param[in]  deviceName name used to decorate outFile.
// TODO maybe deviceName should be optional.
bool
WriteBinaryFile(
    std::ostream& log,
    const std::string& outFile,
    bool isPathDecorationRequired,
    const std::vector<char>& binary,
    const std::string& deviceName);

/// Write a text file.
/// \param      log        a stream for diagnostic output.
/// \param[in]  stem       a stem to use for the file.
/// \param[in]  suffix     a default file suffix (if stem lacks one).
/// \param[in]  text       the chars to write.
/// \param[in]  deviceName device name, used to decorate outFile.
/// \param[in]  kernelName kernel name, used to decorate outFile.
// TODO maybe deviceName should be optional.
bool WriteTextFile(
    std::ostream& log,
    const std::string& stem,
    const std::string& suffix,
    const std::string& text,
    const std::string& deviceName, 
    const std::string& kernelName);



void kaNormelizeDeviceName(const std::string& sOrigin, std::string& sDest);
};

#endif //  KAUtils_Files_H
