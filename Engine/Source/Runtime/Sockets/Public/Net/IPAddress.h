// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "NetUtility.h"

namespace libty::inline Sockets
{
	struct IPAddress
	{
	private:
		union
		{
			uint32 _longAddr;
			uint8 _baddr[4];
		};

	public:
		constexpr IPAddress() noexcept
			: _longAddr(0)
		{
		}

		std::wstring ToString() const
		{
			return String::Format(L"{}.{}.{}.{}", _baddr[0], _baddr[1], _baddr[2], _baddr[3]);
		}

		constexpr uint32 GetAddress() const
		{
			return _longAddr;
		}

		static constexpr IPAddress Any() noexcept
		{
			IPAddress addr;
			addr._longAddr = 0;
			return addr;
		}

		static constexpr IPAddress Loopback() noexcept
		{
			IPAddress addr;
			addr._longAddr = 0x7f000001U;
			return addr;
		}

		static constexpr IPAddress FromIPv4(uint8 b1, uint8 b2, uint8 b3, uint8 b4) noexcept
		{
			IPAddress addr;
			addr._baddr[0] = b1;
			addr._baddr[1] = b2;
			addr._baddr[2] = b3;
			addr._baddr[3] = b4;
			return addr;
		}

		static IPAddress Parse(std::wstring_view ipstring)
		{
			if (ipstring == L"localhost")
			{
				return Loopback();
			}
			else if (ipstring == L"0.0.0.0")
			{
				return Any();
			}

			std::vector<std::wstring> splits = String::Split(ipstring, L".", true, true);
			if (splits.size() != 4)
			{
				throw InvalidOperationException("Source IPString is not valid for parsing IPAddress. There are not 4 components.");
			}

			IPAddress addr;

			try
			{
				for (size_t i = 0; i < 4; ++i)
				{
					int32 value = std::stoi(splits[i], nullptr, 10);
					if (value < 0 || value > 255)
					{
						throw InvalidOperationException("Out of range detected while parsing IP component. Value must between 0 ~ 255.");
					}

					addr._baddr[i] = (uint8)value;
				}
			}
			catch (const std::out_of_range&)
			{
				throw InvalidOperationException("Out of range detected while parsing IP component. Value must between 0 ~ 255.");
			}

			return addr;
		}

		constexpr auto operator <=>(const IPAddress& rhs) const noexcept
		{
			return _longAddr <=> rhs._longAddr;
		}

		constexpr bool operator ==(const IPAddress& rhs) const noexcept
		{
			return _longAddr == rhs._longAddr;
		}

		constexpr IPAddress& operator =(const IPAddress& rhs) noexcept
		{
			_longAddr = rhs._longAddr;
			return *this;
		}
	};
}