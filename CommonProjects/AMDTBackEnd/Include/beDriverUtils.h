//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beDriverUtils.h 
/// 
//=====================================================================
#ifndef __beDriverUtils_h
#define __beDriverUtils_h

// Local.
#include <AMDTBackEnd/Include/beAMDTBackEndDllBuild.h>

// Infra.
#include <AMDTBaseTools/Include/gtString.h>

class KA_BACKEND_DECLDIR beDriverUtils
{
public:
    static bool GetDriverPackagingVersion(gtString& driverPackagingVersion);
private:
    beDriverUtils();
    ~beDriverUtils();
};

#endif // __beDriverUtils_h
