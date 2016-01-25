//=====================================================================
// Copyright 2016 (c), Advanced Micro Devices, Inc. All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file beUtils.cpp 
/// 
//=====================================================================

// C++.
#include <string>

// Infra.
#include <AMDTBaseTools/Include/gtAssert.h>

// Local.
#include <AMDTBackEnd/Include/beUtils.h>

bool beUtils::GdtHwGenToNumericValue(GDT_HW_GENERATION hwGen, size_t& gfxIp)
{
	const size_t BE_GFX_IP_6 = 6;
	const size_t BE_GFX_IP_7 = 7;
	const size_t BE_GFX_IP_8 = 8;

	bool ret = true;
	switch (hwGen)
	{
	case GDT_HW_GENERATION_SOUTHERNISLAND:
		gfxIp = BE_GFX_IP_6;
		break;
	case GDT_HW_GENERATION_SEAISLAND:
		gfxIp = BE_GFX_IP_7;
		break;
	case GDT_HW_GENERATION_VOLCANICISLAND:
		gfxIp = BE_GFX_IP_8;
		break;
	case GDT_HW_GENERATION_NONE:
	case GDT_HW_GENERATION_NVIDIA:
	case GDT_HW_GENERATION_LAST:
	default:
		// We should not get here.
		GT_ASSERT_EX(false, L"Unsupported HW Generation.");
		ret = false;
		break;
	}
	return ret;
}

bool beUtils::GdtHwGenToString(GDT_HW_GENERATION hwGen, std::string& hwGenAsStr)
{
	const char* BE_GFX_IP_6 = "SI";
	const char* BE_GFX_IP_7 = "CI";
	const char* BE_GFX_IP_8 = "VI";

	bool ret = true;
	switch (hwGen)
	{
	case GDT_HW_GENERATION_SOUTHERNISLAND:
		hwGenAsStr = BE_GFX_IP_6;
		break;
	case GDT_HW_GENERATION_SEAISLAND:
		hwGenAsStr = BE_GFX_IP_7;
		break;
	case GDT_HW_GENERATION_VOLCANICISLAND:
		hwGenAsStr = BE_GFX_IP_8;
		break;
	case GDT_HW_GENERATION_NONE:
	case GDT_HW_GENERATION_NVIDIA:
	case GDT_HW_GENERATION_LAST:
	default:
		// We should not get here.
		GT_ASSERT_EX(false, L"Unsupported HW Generation.");
		ret = false;
		break;
	}
	return ret;
}

bool beUtils::GfxCardInfoSortPredicate(const GDT_GfxCardInfo& a, const GDT_GfxCardInfo& b)
{
	// Generation is the primary key.
	if (a.m_generation < b.m_generation) { return true; }

	if (a.m_generation > b.m_generation) { return false; }

	// CAL name is next.
	int ret = ::strcmp(a.m_szCALName, b.m_szCALName);

	if (ret < 0) { return true; }

	if (ret > 0) { return false; }

	// Marketing name next.
	ret = ::strcmp(a.m_szMarketingName, b.m_szMarketingName);

	if (ret < 0) { return true; }

	if (ret > 0) { return false; }

	// DeviceID last.
	return a.m_deviceID < b.m_deviceID;
}

beUtils::beUtils()
{
}


beUtils::~beUtils()
{
}
