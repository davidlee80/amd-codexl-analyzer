//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================// -*- C++ -*-
//=====================================================================
// Copyright 2012 Advanced Micro Devices, Inc. All rights reserved.
//
/// \author GPU Developer Tools
/// \file $File: //devtools/branch/OpenSourceExp/CommonProjects/AMDTBackEnd/Include/beAMDTBackEndDllBuild.h $
/// \version $Revision: #1 $
/// \brief  Thing to decorate exported/imported backend functions.
//
//=====================================================================
// $Id: //devtools/branch/OpenSourceExp/CommonProjects/AMDTBackEnd/Include/beAMDTBackEndDllBuild.h#1 $
// Last checkin:   $DateTime: 2015/12/23 06:14:25 $
// Last edited by: $Author:  AMD Developer Tools Team
// Change list:    $Change: 553348 $
//=====================================================================

// TODO do we need another header for this?  I have duplicates.


#ifdef _WIN32
#pragma warning(disable:4005)
#if defined(AMDTBACKEND_EXPORTS)
    #define KA_BACKEND_DECLDIR __declspec(dllexport)
#elif defined(AMDTANALYSISBACKEND_STATIC)
    #define KA_BACKEND_DECLDIR
#else
    #define KA_BACKEND_DECLDIR __declspec(dllimport)
#endif
#else
// TODO We could use g++ __attribute syntax to control symbol visibility.
#  define KA_BACKEND_DECLDIR
#endif
