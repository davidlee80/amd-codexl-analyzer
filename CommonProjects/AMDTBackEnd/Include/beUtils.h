//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beUtils.h 
/// 
//=====================================================================
#ifndef beUtils_h__
#define beUtils_h__

// Device info.
#include <DeviceInfo.h>

class beUtils
{
public:

	// Convert HW generation to its corresponding numerical value.
	static bool GdtHwGenToNumericValue(GDT_HW_GENERATION hwGen, size_t& gfxIp);

	// Convert HW generation to its corresponding string representation.
	static bool GdtHwGenToString(GDT_HW_GENERATION hwGen, std::string& hwGenAsStr);

	// Predicate to be used for sorting HW devices.
	static bool GfxCardInfoSortPredicate(const GDT_GfxCardInfo& a, const GDT_GfxCardInfo& b);

private:
	// No instances for this class, as this is a static utility class.
	beUtils();
	beUtils(const beUtils& other);
	~beUtils();
};

#endif // beUtils_h__
