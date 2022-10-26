// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPAddress.h"

struct IPEndPoint
{
	IPAddress Address;
	uint16 Port;

	static bool TryParse(const String& addr, IPEndPoint& value)
	{
		std::vector<String> splits = addr.Split(TEXT(":"));
		if (splits.size() != 2)
		{
			return false;
		}

		if (IPAddress::TryParse(splits[0], value.Address) == false)
		{
			return false;
		}

		value.Port = (uint16)String::Stoi(splits[1]);
		return true;
	}
};