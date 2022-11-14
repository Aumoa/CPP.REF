// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/IPAddress.h"
#include <cstring>

IPAddress::IPAddress(uint8 v1, uint8 v2, uint8 v3, uint8 v4)
	: _value{ v1, v2, v3, v4 }
{
}

IPAddress::IPAddress(uint16 v1, uint16 v2, uint16 v3, uint16 v4, uint16 v5, uint16 v6, uint16 v7, uint16 v8)
	: _value(16)
{
	uint16* ptr = reinterpret_cast<uint16*>(_value.data());
	ptr[0] = v1; ptr[1] = v2; ptr[2] = v3; ptr[3] = v4;
	ptr[4] = v5; ptr[5] = v6; ptr[6] = v7; ptr[7] = v8;
}

String IPAddress::ToString() const
{
	if (_value.size() == 0)
	{
		return TEXT("0.0.0.0");
	}
	else if (_value.size() == 4)
	{
		return _ipv4_to_string();
	}
	else if (_value.size() == 16)
	{
		return _ipv6_to_string();
	}

	std::unreachable();
}

bool IPAddress::IsV4() const
{
	return _value.size() != 16;
}

IPAddress IPAddress::Any(bool ipv6)
{
	if (ipv6)
	{
		return IPAddress(0, 0, 0, 0, 0, 0, 0, 0);
	}
	else
	{
		return IPAddress(0, 0, 0, 0);
	}
}

IPAddress IPAddress::Loopback(bool ipv6)
{
	if (ipv6)
	{
		return IPAddress(0, 0, 0, 0, 0, 0, 0, 1);
	}
	else
	{
		return IPAddress(127, 0, 0, 1);
	}
}

String IPAddress::_ipv4_to_string() const
{
	return TEXTF("{}.{}.{}.{}", _value[0], _value[1], _value[2], _value[3]);
}

String IPAddress::_ipv6_to_string() const
{
	const uint16* ptr = reinterpret_cast<const uint16*>(_value.data());
	constexpr uint16 any_loopback[7] = {};

	// early return cases
	if (memcmp(ptr, any_loopback, sizeof(any_loopback)) == 0)
	{
		if (ptr[7] == 0)
		{
			// any
			return TEXT("::");
		}
		else if (ptr[7] == 1)
		{
			// loopback
			return TEXT("::1");
		}
	}

	std::wstringstream wss;
	wss << std::hex;
	int shrink = 0;

	for (size_t i = 0; i < 8; ++i)
	{
		if (ptr[i] == 0)
		{
			if (shrink == 0)
			{
				// Write empty scope.
				wss << ':';
				shrink = 1;
			}
			else if (shrink == 2)
			{
				wss << ":0";
			}
		}
		else
		{
			if (shrink == 1)
			{
				shrink = 2;
			}

			if (i > 0)
			{
				wss << ':';
			}

			wss << ptr[i];
		}
	}

	return String(wss.str());
}