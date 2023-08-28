// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/String.h"
#include "Platform/PlatformLocalization.h"
#include <random>

class CORE_API Guid
{
	uint32 Data1;
	uint16 Data2;
	uint16 Data3;
	std::array<uint8, 8> Data4;

public:
	constexpr Guid()
		: Data1(0)
		, Data2(0)
		, Data3(0)
		, Data4{}
	{
	}

	constexpr Guid(uint32 Data1, uint16 Data2, uint16 Data3, uint8 Data40, uint8 Data41, uint8 Data42, uint8 Data43, uint8 Data44, uint8 Data45, uint8 Data46, uint8 Data47)
		: Data1(Data1)
		, Data2(Data2)
		, Data3(Data3)
		, Data4{ Data40, Data41, Data42, Data43, Data44, Data45, Data46, Data47 }
	{
	}

	String ToString() const
	{
		return String::Format(TEXT("{:08X}-{:04X}-{:04X}-{:016X}"), Data1, Data2, Data3, *reinterpret_cast<const uint64*>(Data4.data()));
	}

	static bool TryParse(String FormattedString, Guid& OutResult)
	{
		std::vector<String> Datas = FormattedString.Split(TEXT("-"), EStringSplitOptions::TrimEntries | EStringSplitOptions::RemoveEmptyEntries);
		if (Datas.size() != 4)
		{
			OutResult = {};
			return false;
		}

		if (UInt32::TryParse(Datas[0], OutResult.Data1, 16) == false)
		{
			OutResult = {};
			return false;
		}
		if (UInt16::TryParse(Datas[1], OutResult.Data2, 16) == false)
		{
			OutResult = {};
			return false;
		}
		if (UInt16::TryParse(Datas[2], OutResult.Data3, 16) == false)
		{
			OutResult = {};
			return false;
		}
		if (UInt64::TryParse(Datas[3], *reinterpret_cast<uint64*>(OutResult.Data4.data()), 16) == false)
		{
			OutResult = {};
			return false;
		}
		return true;
	}

	static Guid NewGuid()
	{
		static std::random_device Rnd;
		static std::mt19937_64 Gen(Rnd());

		Guid Instance;
		uint64* RawPtr = reinterpret_cast<uint64*>(&Instance.Data1);
		RawPtr[0] = Gen();
		RawPtr[1] = Gen();
		return Instance;
	}
};