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

	constexpr IPAddress(uint8 b1, uint8 b2, uint8 b3, uint8 b4)
		: Address{ b1, b2, b3, b3 }
	{
	}

	static IPAddress Parse(const String& ipv4Addr)
	{
		if (ipv4Addr == TEXT("localhost"))
		{
			return IPAddress::Loopback();
		}
		else if (ipv4Addr == TEXT("*"))
		{
			return IPAddress::Any();
		}
		else
		{
			std::vector<String> bytes = ipv4Addr.Split(L'.', EStringSplitOptions::TrimEntries | EStringSplitOptions::RemoveEmptyEntries);
			if (bytes.size() != 4)
			{
				throw ArgumentException(TEXT("Invalid ipv4 address string."));
			}

			std::wstringstream wss((std::wstring)String::Join(TEXT(" "), bytes));
			int32 b1, b2, b3, b4;
			wss >> b1 >> b2 >> b3 >> b4;

			int32 min = std::min({ b1, b2, b3, b4 });
			int32 max = std::max({ b1, b2, b3, b4 });
			if (min < 0 || max > 255)
			{
				throw ArgumentException(TEXT("Invalid pv4 address string."));
			}

			return IPAddress((uint8)b1, (uint8)b2, (uint8)b3, (uint8)b4);
		}
	}
};