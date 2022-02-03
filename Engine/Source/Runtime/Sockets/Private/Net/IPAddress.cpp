// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/IPAddress.h"
#include <exception>

#if PLATFORM_WINDOWS

#include <WinSock2.h>
#include <WS2tcpip.h>

#endif

std::wstring IPAddress::ToString() const
{
	auto Conv = ntohl(Address);
	if (Conv == INADDR_ANY)
	{
		return L"0.0.0.0";
	}
	else if (Conv == INADDR_LOOPBACK)
	{
		return L"localhost";
	}
	else
	{
		return std::format(L"{}.{}.{}.{}", ByteAddress[0], ByteAddress[1], ByteAddress[2], ByteAddress[3]);
	}
}

IPAddress IPAddress::Any()
{
	static IPAddress AddrAny = []()
	{
		auto Conv = htonl(INADDR_ANY);
		IPAddress Addr;
		memcpy(Addr.ByteAddress, &Conv, sizeof(Conv));
		return Addr;
	}();

	return AddrAny;
}

IPAddress IPAddress::Loopback()
{
	static IPAddress AddrLoopback = []()
	{
		auto Conv = htonl(INADDR_LOOPBACK);
		IPAddress Addr;
		memcpy(Addr.ByteAddress, &Conv, sizeof(Conv));
		return Addr;
	}();

	return AddrLoopback;
}

IPAddress IPAddress::FromIPv4(uint8 V1, uint8 V2, uint8 V3, uint8 V4)
{
	IPAddress Addr;
	Addr.Address = htonl(((uint32)V1 << 24) | ((uint32)V2 << 16) | ((uint32)V3 << 8) | ((uint32)V4));
	return Addr;
}

bool IPAddress::TryParse(std::wstring_view IPString, IPAddress* RefAddress)
{
	if (IPString == L"localhost" || IPString == L"0.0.0.0")
	{
		*RefAddress = Loopback();
		return true;
	}
	else if (IPString == L"")
	{
		*RefAddress = Any();
		return true;
	}
	else
	{
		std::vector<std::wstring> Bytes = StringUtils::Split(IPString, L".", true, true);
		if (Bytes.size() != 4)
		{
			*RefAddress = Any();
			return false;
		}

		int32 ByteAddr[4] = {};
		try
		{
			for (size_t i = 0; i < 4; ++i)
			{
				ByteAddr[i] = std::stoi(Bytes[i]);

				if (ByteAddr[i] < 0 || ByteAddr[i] > 255)
				{
					throw std::out_of_range("Component of IPAddress of string must in between 0 and 255.");
				}
			}
		}
		catch (const std::exception&)
		{
			*RefAddress = Any();
			return false;
		}

		for (size_t i = 0; i < 4; ++i)
		{
			RefAddress->ByteAddress[i] = (uint8)ByteAddr[i];
		}

		return true;
	}
}

bool IPAddress::TryParseHostname(std::wstring_view Hostname, std::vector<IPAddress>* RefAddresses)
{
	std::string AHostname = WCHAR_TO_ANSI(Hostname);
	struct hostent* Entry = gethostbyname(AHostname.c_str());

	if (Entry->h_addr_list == nullptr)
	{
		return false;
	}

	size_t Count = 0;
	for (Count = 0; Entry->h_addr_list[Count]; ++Count);

	RefAddresses->reserve(Count);
	for (size_t i = 0; i < Count; ++i)
	{
		RefAddresses->emplace_back(IPAddress{ .Address = *reinterpret_cast<const uint32*>(Entry->h_addr_list[i]) });
	}

	return true;
}