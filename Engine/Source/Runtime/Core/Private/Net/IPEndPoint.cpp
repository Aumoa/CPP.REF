// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/IPEndPoint.h"
#include "Misc/String.h"

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#endif

IPEndPoint::IPEndPoint(const IPAddress& ipaddr, uint16 port)
	: _addr(ipaddr)
	, _port(htons(port))
{
}

String IPEndPoint::ToString() const
{
	if (_addr.IsV4())
	{
		return TEXTF("{}:{}", _addr.ToString(), ntohs(_port));
	}
	else
	{
		return TEXTF("[{}]:{}", _addr.ToString(), ntohs(_port));
	}
}

void IPEndPoint::ApplyTo(sockaddr_buf& buf) const
{
	if (_addr.IsV4())
	{
		auto& sin = reinterpret_cast<sockaddr_in&>(buf);
		sin.sin_family = AF_INET;
		memcpy(&sin.sin_addr, _addr.GetBytes().data(), sizeof(uint8) * 4);
		sin.sin_port = _port;
	}
	else
	{
		auto& sin = reinterpret_cast<sockaddr_in6&>(buf);
		sin.sin6_family = AF_INET6;
		memcpy(&sin.sin6_addr, _addr.GetBytes().data(), sizeof(uint16) * 8);
		sin.sin6_port = _port;
		sin.sin6_flowinfo = 0;
		sin.sin6_scope_id = 0;
	}
}

size_t IPEndPoint::Size() const
{
	return _addr.IsV4() ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
}