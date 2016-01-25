//=====================================================================
// Copyright 2008-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file StringUtils.h 
/// 
//=====================================================================

//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/Common/Src/ShaderUtils/StringUtils.h#1 $
//
// Last checkin:  $DateTime: 2016/01/05 04:19:10 $
// Last edited by: $Author:  AMD Developer Tools Team
//=====================================================================

#pragma once

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

/// StringUtils is a set of string utility functions.

namespace StringUtils
{
/// Split a string into a vector of line strings.
/// \param[in] strString The string to split into lines.
/// \param[out] vLines The vector of lines.
/// \param[in] bTrimLines Trim white space from each line.
/// \return The number of lines.
size_t SplitStringIntoLines(const std::string& strString, std::vector<std::string>& vLines, bool bTrimLines = false);

/// Trim white space from the start & end of a string.
/// \param[in, out] strString The string to trim.
void TrimString(std::string& strString);

/// Find the entry function name by scanning the source
/// \param[in] strFunctionEntry  the function entry name
/// \param[in] strSource         the shader source string
/// \return the line number (0 if it fails).
size_t FindFunctionEntryInString(const std::string& strFunctionEntry, const std::string& strSource);
}; // StringUtils


#endif // STRINGUTILS_H
