// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Platform/Generic/GenericPlatformLocalization.h"
#include "IntegralTypes.h"
#include <string>
#include <string_view>

namespace Ayla
{
	struct CORE_API WindowsPlatformLocalization : public GenericPlatformLocalization
	{
		static std::wstring FromCodepage(std::string_view Str, int32 InCodepage = 0);
		static std::string ToCodepage(std::wstring_view Wstr, int32 InCodepage = 0);
	};

	using PlatformLocalization = WindowsPlatformLocalization;
}

#endif