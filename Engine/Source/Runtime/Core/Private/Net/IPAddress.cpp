// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/IPAddress.h"
#include "ArgumentException.h"
#include "FormatException.h"
#include <algorithm>
#include <sstream>

namespace Ayla
{
	IPAddress IPAddress::Parse(const String& address)
	{
		IPAddress result;
		if (!TryParse(address, result))
		{
			throw FormatException(TEXT("Invalid IP address format"));
		}
		return result;
	}

	bool IPAddress::TryParse(const String& address, IPAddress& result)
	{
		if (address.IsEmpty())
		{
			return false;
		}

		// Try IPv4 first
		if (TryParseIPv4(address, result))
		{
			return true;
		}

		// Try IPv6
		if (TryParseIPv6(address, result))
		{
			return true;
		}

		return false;
	}

	String IPAddress::ToString() const
	{
		if (m_Family == AddressFamily::InterNetwork)
		{
			return String::Format(TEXT("{}.{}.{}.{}"), 
				m_AddressBytes[0], m_AddressBytes[1], 
				m_AddressBytes[2], m_AddressBytes[3]);
		}
		else
		{
			// IPv6 formatting - basic implementation
			// Check for loopback
			bool isLoopback = true;
			for (size_t i = 0; i < 15; ++i)
			{
				if (m_AddressBytes[i] != 0)
				{
					isLoopback = false;
					break;
				}
			}
			if (isLoopback && m_AddressBytes[15] == 1)
			{
				return TEXT("::1");
			}

			// Check for any (all zeros)
			bool isAny = true;
			for (size_t i = 0; i < 16; ++i)
			{
				if (m_AddressBytes[i] != 0)
				{
					isAny = false;
					break;
				}
			}
			if (isAny)
			{
				return TEXT("::");
			}

			// Basic IPv6 formatting (without compression optimization)
			String result;
			for (size_t i = 0; i < 16; i += 2)
			{
				if (i > 0)
				{
					result += TEXT(":");
				}
				uint16 group = (static_cast<uint16>(m_AddressBytes[i]) << 8) | m_AddressBytes[i + 1];
				result += String::Format(TEXT("{:x}"), group);
			}
			return result;
		}
	}

	bool IPAddress::TryParseIPv4(const String& address, IPAddress& result)
	{
		// Split by '.'
		auto parts = address.SplitView('.');
		if (parts.size() != 4)
		{
			return false;
		}

		std::array<uint8, 4> bytes;
		for (size_t i = 0; i < 4; ++i)
		{
			if (!IsValidIPv4Octet(parts[i], bytes[i]))
			{
				return false;
			}
		}

		result = IPAddress(bytes);
		return true;
	}

	bool IPAddress::TryParseIPv6(const String& address, IPAddress& result)
	{
		// Simple IPv6 parsing - handle basic cases
		if (address == TEXT("::"))
		{
			result = IPv6Any();
			return true;
		}
		
		if (address == TEXT("::1"))
		{
			result = IPv6Loopback();
			return true;
		}

		// More comprehensive IPv6 parsing would go here
		// For now, just handle the basic cases above
		return false;
	}

	bool IPAddress::IsValidIPv4Octet(const String& octet, uint8& value)
	{
		// Check for leading zeros (except "0" itself)
		if (octet.length() > 1 && octet[0] == L'0')
		{
			return false;
		}

		// Convert to integer
		return UInt8::TryParse(octet, value, 10);
	}

	bool IPAddress::IsValidIPv6Hex(const String& hex, uint16& value)
	{
		return UInt16::TryParse(hex, value, 16);
	}
}