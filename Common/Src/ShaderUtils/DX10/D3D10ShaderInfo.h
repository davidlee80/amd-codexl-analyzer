//=====================================================================
// Copyright 2007-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file D3D10ShaderInfo.h 
/// 
//=====================================================================

//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/Common/Src/ShaderUtils/DX10/D3D10ShaderInfo.h#1 $
//
// Last checkin:  $DateTime: 2016/01/05 04:19:10 $
// Last edited by: $Author:  AMD Developer Tools Team
//=====================================================================

#pragma once

#ifndef D3D10SHADERINFO_H
#define D3D10SHADERINFO_H

#include <D3D10_1.h>
#include <string>
#include "../DX11/d3d11tokenizedprogramformat.hpp"
#include "D3D10ShaderObject.h"

/// D3D10ShaderInfo is a set of functions for querying D3D10 shader debug information.

namespace D3D10ShaderInfo
{
/// Get a string describing the variable type.
/// \param[in] type  The variable type.
/// \return    The variable type description.
const char* GetVarType(D3D10_SHADER_VARIABLE_TYPE type);

/// Dump data from the shader debug chunk to a string.
/// \param[in]    pSDBGChunk     A pointer to the shader debug info chunk.
/// \param[out]   strDebugInfo   A text dump of the shader debug info.
bool DumpDX10ShaderDebugInfo(const D3D10ShaderObject::D3D10_ChunkHeader* pSDBGChunk, std::string& strDebugInfo);

}; // D3D10ShaderInfo

#endif // D3D10SHADERINFO_H
