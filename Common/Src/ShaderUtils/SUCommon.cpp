//=====================================================================
// Copyright (c) 2010 Advanced Micro Devices, Inc. All rights reserved.
//
/// \author GPU Developer Tools
/// \file $File: //devtools/branch/OpenSourceExp/Common/Src/ShaderUtils/SUCommon.cpp $
/// \version $Revision: #1 $
/// \brief  This file defines enums and structs used in ShaderDebugger
///         and APP Profiler.
//
//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/Common/Src/ShaderUtils/SUCommon.cpp#1 $
// Last checkin:   $DateTime: 2016/01/05 04:19:10 $
// Last edited by: $Author:  AMD Developer Tools Team
// Change list:    $Change: 553959 $
//=====================================================================

#include "SUCommon.h"

using namespace ShaderUtils;

const ThreadID ShaderUtils::g_tID_Error = ThreadID(TID_2D, -1, -1, -1, -1);

bool ShaderUtils::IsValid(const ThreadID& threadID)
{
    switch (threadID.type)
    {
        case TID_1D: return (threadID.x >= 0);

        case TID_2D: return (threadID.x >= 0 && threadID.y >= 0);

        case TID_3D: return (threadID.x >= 0 && threadID.y >= 0 && threadID.z >= 0);

        default:     break;
    }

    return false;
}
