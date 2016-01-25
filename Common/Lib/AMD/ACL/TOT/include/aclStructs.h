//=====================================================================
// Copyright 2012-2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file 
/// 
//=====================================================================
// This is a compatibility header file. Either define the version
// of the compiler library that is to be used or include the
// header file for that version directly.
#ifndef ACL_STRUCTS_H_
#define ACL_STRUCTS_H_
#if WITH_VERSION_0_8
#include "v0_8/aclStructs.h"
#elif WITH_VERSION_0_9
#else
#error "The compiler library version was not defined."
#include "v0_8/aclStructs.h"
#endif
#endif // ACL_STRUCTS_H_
