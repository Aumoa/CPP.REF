// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Exceptions/ArgumentException.h"

struct IPAddress
{
	union
	{
		uint8 Address[4];
		uint32 Long;
	};

	static constexpr IPAddress Any()
	{
		return IPAddress(0, 0, 0, 0);
	}

	static constexpr IPAddress Loopback()
	{
		return IPAddress(127, 0, 0, 1);
	}

	constexpr IPAddress(uint8 b1 = 0, uint8 b2 = 0, uint8 b3 = 0, uint8 b4 = 0)
		: Address{ b1, b2, b3, b4 }
	{
	}

	static bool TryParse(const String& ipv4Addr, IPAddress& value)
	{
		if (ipv4Addr == TEXT("localhost"))
		{
			value = IPAddress::Loopback();
			return true;
		}
		else if (ipv4Addr == TEXT("*"))
		{
			value = IPAddress::Any();
			return true;
		}
		else
		{
			std::vector<String> bytes = ipv4Addr.Split(L'.', EStringSplitOptions::TrimEntries | EStringSplitOptions::RemoveEmptyEntries);
			if (bytes.size() != 4)
			{
				return false;
			}

			std::wstringstream wss((std::wstring)String::Join(TEXT(" "), bytes));
			int32 b1, b2, b3, b4;
			wss >> b1 >> b2 >> b3 >> b4;

			int32 min = std::min({ b1, b2, b3, b4 });
			int32 max = std::max({ b1, b2, b3, b4 });
			if (min < 0 || max > 255)
			{
				return false;
			}

			value = IPAddress((uint8)b1, (uint8)b2, (uint8)b3, (uint8)b4);
			return true;
		}
	}
};