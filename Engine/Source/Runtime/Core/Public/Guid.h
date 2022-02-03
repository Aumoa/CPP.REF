// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

struct CORE_API Guid
{
	uint32 Data1;
	uint16 Data2;
	uint16 Data3;
	uint8 Data4[8];

	constexpr Guid()
		: Data1(0)
		, Data2(0)
		, Data3(0)
		, Data4{}
	{
	}

	constexpr Guid(uint32 Data1, uint16 Data2, uint16 Data3, uint8(&Data4)[8])
		: Data1(Data1)
		, Data2(Data2)
		, Data3(Data3)
		, Data4{ Data4[0], Data4[1], Data4[2], Data4[3], Data4[4], Data4[5], Data4[6], Data4[7] }
	{
	}

	constexpr Guid(uint32 Data1, uint16 Data2, uint16 Data3, uint8 Data40, uint8 Data41, uint8 Data42, uint8 Data43, uint8 Data44, uint8 Data45, uint8 Data46, uint8 Data47)
		: Data1(Data1)
		, Data2(Data2)
		, Data3(Data3)
		, Data4{ Data40, Data41, Data42, Data43, Data44, Data45, Data46, Data47 }
	{
	}

	std::wstring ToString() const;

	static Guid FromString(std::wstring_view InGuidFormat);
	static Guid NewGuid();

	constexpr auto operator <=>(const Guid& Rhs) const = default;
};