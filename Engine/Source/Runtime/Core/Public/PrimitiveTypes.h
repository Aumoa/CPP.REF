// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <chrono>
#include <string>
#include <complex>
#include <string_view>

namespace libty::inline Core::inline Literals
{
	using int8 = signed char;
	using int16 = signed short;
	using int32 = signed long;
	using int64 = signed long long;
	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned long;
	using uint64 = unsigned long long;
}

using namespace std::literals;
using namespace libty::Core::Literals;