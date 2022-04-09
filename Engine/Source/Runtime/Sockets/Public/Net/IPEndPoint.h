// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IPAddress.h"
#include "NetUtility.h"

namespace libty::inline Sockets
{
	struct SOCKETS_API IPEndPoint
	{
	private:
		IPAddress _address;
		uint16 _port;

	public:
		constexpr IPEndPoint() noexcept
			: _port(0)
		{
		}

		constexpr IPEndPoint(const IPAddress& addr, uint16 port)
			: _address(addr)
			, _port(port)
		{
		}

		constexpr uint32 GetAddress() const
		{
			return _address.GetAddress();
		}

		constexpr uint16 GetPort() const
		{
			return _port;
		}

		std::wstring ToString() const
		{
			return String::Format(L"{}:{}", _address, _port);
		}

		static IPEndPoint Parse(std::wstring_view epstr)
		{
			if (epstr.find(L':') != std::wstring_view::npos)
			{
				IPEndPoint ep;

				std::vector<std::wstring> splits = String::Split(epstr , L":", true, true);
				ep._address = IPAddress::Parse(splits[0]);
				
				int32 value = std::stoi(splits[1]);
				if (value < 0 || value > 65535)
				{
					throw InvalidOperationException("Out of range detected while parsing port component. Value must between 0 ~ 65535.");
				}

				ep._port = (uint16)value;
				return ep;
			}

			throw InvalidOperationException("Port is not specified.");
		}
	};
}