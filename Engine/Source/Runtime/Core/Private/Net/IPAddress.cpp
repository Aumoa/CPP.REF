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

		// Check for IPv4-mapped IPv6 addresses (::ffff:192.168.1.1)
		if (address.StartsWith(TEXT("::ffff:")) && address.length() > 7)
		{
			String ipv4Part = address.SubstringView(7);
			IPAddress ipv4Addr;
			if (TryParseIPv4(ipv4Part, ipv4Addr))
			{
				// Create IPv4-mapped IPv6 address
				std::array<uint8, 16> bytes{};
				bytes[10] = 0xFF;
				bytes[11] = 0xFF;
				auto ipv4Bytes = ipv4Addr.GetAddressBytes();
				std::copy(ipv4Bytes.begin(), ipv4Bytes.end(), bytes.begin() + 12);
				result = IPAddress(bytes);
				return true;
			}
		}

		// Check for IPv4-compatible IPv6 addresses (::192.168.1.1)
		if (address.StartsWith(TEXT("::")) && address.length() > 2 && 
			address[2] != ':' && address.Contains('.'))
		{
			String ipv4Part = address.SubstringView(2);
			IPAddress ipv4Addr;
			if (TryParseIPv4(ipv4Part, ipv4Addr))
			{
				// Create IPv4-compatible IPv6 address
				std::array<uint8, 16> bytes{};
				auto ipv4Bytes = ipv4Addr.GetAddressBytes();
				std::copy(ipv4Bytes.begin(), ipv4Bytes.end(), bytes.begin() + 12);
				result = IPAddress(bytes);
				return true;
			}
		}

		// Parse standard IPv6 addresses with possible compression
		std::array<uint16, 8> groups{};
		
		// Check for double colon compression
		size_t doubleColonIndex = address.IndexOf(TEXT("::"));
		if (doubleColonIndex != (size_t)-1)
		{
			// Ensure there's only one "::" in the address
			if (address.IndexOf(TEXT("::"), doubleColonIndex + 2) != (size_t)-1)
			{
				return false;
			}
			
			// Split into parts before and after "::"
			String beforeColon = address.SubstringView(0, doubleColonIndex);
			String afterColon = address.SubstringView(doubleColonIndex + 2);
			
			// Parse groups before "::"
			size_t groupsBefore = 0;
			if (!beforeColon.IsEmpty())
			{
				auto beforeParts = beforeColon.SplitView(':');
				if (beforeParts.size() > 7)
				{
					return false;
				}
				for (size_t i = 0; i < beforeParts.size(); ++i)
				{
					if (beforeParts[i].IsEmpty() || beforeParts[i].length() > 4 || 
						!IsValidIPv6Hex(beforeParts[i], groups[i]))
					{
						return false;
					}
				}
				groupsBefore = beforeParts.size();
			}

			// Parse groups after "::"
			size_t groupsAfter = 0;
			if (!afterColon.IsEmpty())
			{
				auto afterParts = afterColon.SplitView(':');
				if (afterParts.size() > (8 - groupsBefore))
				{
					return false;
				}
				
				size_t startIndex = 8 - afterParts.size();
				for (size_t i = 0; i < afterParts.size(); ++i)
				{
					if (afterParts[i].IsEmpty() || afterParts[i].length() > 4 || 
						!IsValidIPv6Hex(afterParts[i], groups[startIndex + i]))
					{
						return false;
					}
				}
				groupsAfter = afterParts.size();
			}

			// Check if compression is valid (there must be at least one zero group)
			if (groupsBefore + groupsAfter >= 8)
			{
				return false;
			}
		}
		else
		{
			// No compression - must have exactly 8 groups
			auto parts = address.SplitView(':');
			if (parts.size() != 8)
			{
				return false;
			}

			for (size_t i = 0; i < 8; ++i)
			{
				if (parts[i].IsEmpty() || parts[i].length() > 4 || 
					!IsValidIPv6Hex(parts[i], groups[i]))
				{
					return false;
				}
			}
		}

		// Convert groups to byte array
		std::array<uint8, 16> bytes;
		for (size_t i = 0; i < 8; ++i)
		{
			bytes[i * 2] = static_cast<uint8>((groups[i] >> 8) & 0xFF);
			bytes[i * 2 + 1] = static_cast<uint8>(groups[i] & 0xFF);
		}

		result = IPAddress(bytes);
		return true;
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