// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/CrcHash.h"

class GuidHelper
{
	static constexpr std::wstring_view BaseGuid = L"E14EE145-A04A-44CE-{:0>2X}{:0>2X}-{:0>2X}{:0>2X}{:0>2X}{:0>2X}{:0>2X}{:0>2X}";

public:
	static Guid GenerateGuid(std::wstring_view Name)
	{
		std::string A_Name = WCHAR_TO_ANSI(Name);
		uint64 CrcHash64 = CrcHash::Hash64(A_Name.c_str(), A_Name.length());
		uint8* GuidBytes = reinterpret_cast<uint8*>(&CrcHash64);

		return Guid::FromString(std::format(BaseGuid, GuidBytes[0], GuidBytes[1], GuidBytes[2], GuidBytes[3], GuidBytes[4], GuidBytes[5], GuidBytes[6], GuidBytes[7]));
	}
};