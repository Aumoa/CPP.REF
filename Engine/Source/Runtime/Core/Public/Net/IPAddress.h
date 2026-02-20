// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Platform/PlatformTypes.h"
#include "Net/Sockets/AddressFamily.h"
#include "IntegralTypes.h"
#include "String_.h"
#include <array>
#include <span>

namespace Ayla
{
	class CORE_API IPAddress
	{
	private:
		AddressFamily m_Family;
		std::array<uint8, 16> m_AddressBytes;

	public:
		constexpr IPAddress() noexcept
			: m_Family(AddressFamily::InterNetwork)
			, m_AddressBytes{}
		{
		}

		constexpr IPAddress(const std::array<uint8, 4>& address) noexcept
			: m_Family(AddressFamily::InterNetwork)
			, m_AddressBytes{}
		{
			std::copy(address.begin(), address.end(), m_AddressBytes.begin());
		}

		constexpr IPAddress(uint8 a1, uint8 a2, uint8 a3, uint8 a4) noexcept
			: m_Family(AddressFamily::InterNetwork)
			, m_AddressBytes{ a1, a2, a3, a4 }
		{
		}

		constexpr IPAddress(const std::array<uint8, 16>& address) noexcept
			: m_Family(AddressFamily::InterNetworkV6)
		{
			std::copy(address.begin(), address.end(), m_AddressBytes.begin());
		}

		constexpr IPAddress(uint8 a1, uint8 a2, uint8 a3, uint8 a4, uint8 a5, uint8 a6, uint8 a7, uint8 a8,
			uint8 a9, uint8 a10, uint8 a11, uint8 a12, uint8 a13, uint8 a14, uint8 a15, uint8 a16) noexcept
			: m_Family(AddressFamily::InterNetworkV6)
			, m_AddressBytes{ a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16 }
		{
		}

		constexpr IPAddress(uint32 address) noexcept
			: m_Family(AddressFamily::InterNetwork)
			, m_AddressBytes{ static_cast<uint8>(address & 0xFF), static_cast<uint8>((address >> 8) & 0xFF), static_cast<uint8>((address >> 16) & 0xFF), static_cast<uint8>((address >> 24) & 0xFF) }
		{
		}

		constexpr AddressFamily GetAddressFamily() const noexcept
		{
			return m_Family;
		}

		constexpr std::span<const uint8> GetAddressBytes() const noexcept
		{
			if (m_Family == AddressFamily::InterNetwork)
			{
				return std::span<const uint8>(m_AddressBytes.data(), 4);
			}
			else
			{
				return std::span<const uint8>(m_AddressBytes.data(), 16);
			}
		}
		
		static constexpr IPAddress Any() noexcept
		{
			static constexpr std::array<uint8, 4> addr = {};
			return IPAddress(addr);
		}

		static constexpr IPAddress IPv6Any() noexcept
		{
			static constexpr std::array<uint8, 16> addr = {};
			return IPAddress(addr);
		}

		static constexpr IPAddress Loopback() noexcept
		{
			static constexpr std::array<uint8, 4> addr = { 127, 0, 0, 1 };
			return IPAddress(addr);
		}

		static constexpr IPAddress IPv6Loopback() noexcept
		{
			static constexpr std::array<uint8, 16> addr = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
			return IPAddress(addr);
		}
		
		static IPAddress Parse(const String& address);
		static bool TryParse(const String& address, IPAddress& result);

		String ToString() const;

		constexpr auto operator <=>(const IPAddress& other) const noexcept
		{
			auto comp = m_Family <=> other.m_Family;
			if (comp != 0)
			{
				return comp;
			}

			size_t count = (m_Family == AddressFamily::InterNetwork) ? 4 : 16;
			for (size_t i = 0; i < count; ++i)
			{
				comp = m_AddressBytes[i] <=> other.m_AddressBytes[i];
				if (comp != 0)
				{
					return comp;
				}
			}

			return std::strong_ordering::equal;
		}

		constexpr auto operator ==(const IPAddress& other) const noexcept
		{
			return (*this <=> other) == 0;
		}

		constexpr auto operator !=(const IPAddress& other) const noexcept
		{
			return !(*this == other);
		}

	private:
		static bool TryParseIPv4(const String& address, IPAddress& result);
		static bool TryParseIPv6(const String& address, IPAddress& result);
		static bool IsValidIPv4Octet(const String& octet, uint8& value);
		static bool IsValidIPv6Hex(const String& hex, uint16& value);
	};
}