// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/IPAddress.h"
#include "Exceptions/FormatException.h"
#include <cstring>

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#endif

IPAddress::IPAddress(uint8 v1, uint8 v2, uint8 v3, uint8 v4)
	: _value{ v1, v2, v3, v4 }
{
}

IPAddress::IPAddress(uint16 v1, uint16 v2, uint16 v3, uint16 v4, uint16 v5, uint16 v6, uint16 v7, uint16 v8)
	: _v6(true)
{
	uint16* ptr = reinterpret_cast<uint16*>(_value.data());
	ptr[0] = htons(v1); ptr[1] = htons(v2); ptr[2] = htons(v3); ptr[3] = htons(v4);
	ptr[4] = htons(v5); ptr[5] = htons(v6); ptr[6] = htons(v7); ptr[7] = htons(v8);
}

IPAddress::IPAddress(String str)
{
	if (TryParse(str, this) == false)
	{
		throw FormatException(TEXT("Invalid IP address format."));
	}
}

String IPAddress::ToString() const
{
#if PLATFORM_WINDOWS
	static thread_local WCHAR sBuf[32 + 8];
	InetNtopW(_v6 ? AF_INET6 : AF_INET, _value.data(), sBuf, 32 + 8);
	return String(sBuf);
#endif
}

bool IPAddress::IsV4() const noexcept
{
	return _v6 == false;
}

std::span<const uint8> IPAddress::GetBytes() const noexcept
{
	return _value;
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

bool IPAddress::TryParse(String str, IPAddress* result) noexcept
{
	int& errno_val = errno;

	if (result == nullptr)
	{
		// invalid argument.
		errno_val = EINVAL;
		return false;
	}

	if (str.length() < 2)
	{
		errno_val = EINVAL;
		return false;
	}

	bool ipv4 = false;
	for (size_t i = 0; i < std::min((size_t)4, str.length()); ++i)
	{
		if (str[i] == '.')
		{
			ipv4 = true;
			break;
		}
	}

	result->_v6 = !ipv4;

#if PLATFORM_WINDOWS
	return InetPtonW(ipv4 ? AF_INET : AF_INET6, str.c_str(), result->_value.data()) == 1;
#endif
}