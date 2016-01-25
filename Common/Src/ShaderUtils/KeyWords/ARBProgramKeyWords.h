//=====================================================================
// Copyright 2009-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================
//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/Common/Src/ShaderUtils/KeyWords/ARBProgramKeyWords.h#1 $
//
// Last checkin:  $DateTime: 2016/01/05 04:19:10 $
// Last edited by: $Author:  AMD Developer Tools Team
//=====================================================================


#pragma once

#ifndef ARBPROGRAMKEYWORDS_H
#define ARBPROGRAMKEYWORDS_H

#include "KeyWords.h"

/// ShaderUtils is a set of shader utility functions.

namespace ShaderUtils
{
/// Get the keywords list for ARB FP shaders
/// \param[out] keyWords The keywords list.
/// \return True if successful, otherwise false.
bool GetARBFPKeyWords(ShaderUtils::KeyWordsList& keyWords);

/// Get the keywords list for ARB VP shaders
/// \param[out] keyWords The keywords list.
/// \return True if successful, otherwise false.
bool GetARBVPKeyWords(ShaderUtils::KeyWordsList& keyWords);
}; // ShaderUtils

#endif // ARBPROGRAMKEYWORDS_H
