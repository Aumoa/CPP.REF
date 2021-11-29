// Copyright 2020-2021 Aumoa.lib. All right reserved.

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

	std::wstring ToString() const;

	static Guid FromString(std::wstring_view InGuidFormat);
	static Guid NewGuid();
};