// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Net/IPEndPoint.h"

#if PLATFORM_WINDOWS

#include <WinSock2.h>
#include <WS2tcpip.h>

#endif

IPEndPoint::IPEndPoint()
	: IP(IPAddress::Any())
	, NetPort(0)
{
}

IPEndPoint::IPEndPoint(const IPAddress& IP, uint16 Port)
	: IP(IP)
	, NetPort(0)
{
	SetPort(Port);
}

void IPEndPoint::SetPort(uint16 InPort)
{
	NetPort = htons(InPort);
}

uint16 IPEndPoint::GetPort() const
{
	return ntohs(NetPort);
}

uint16 IPEndPoint::GetNetPort() const
{
	return NetPort;
}

std::wstring IPEndPoint::ToString() const
{
	if (NetPort == 0)
	{
		return IP.ToString();
	}
	else
	{
		return std::format(L"{}:{}", IP.ToString(), GetPort());
	}
}

bool IPEndPoint::TryParse(std::wstring_view IPString, IPEndPoint* RefEndPoint)
{
	if (IPString.find(L':') != std::wstring::npos)
	{
		std::vector<std::wstring> Splits = StringUtils::Split(IPString, L":", true, true);
		if (!IPAddress::TryParse(Splits[0], &RefEndPoint->IP))
		{
			return false;
		}

		RefEndPoint->SetPort(std::stoi(Splits[1]));
		return true;
	}
	else
	{
		if (!IPAddress::TryParse(IPString, &RefEndPoint->IP))
		{
			return false;
		}

		RefEndPoint->NetPort = 0;
		return true;
	}
}