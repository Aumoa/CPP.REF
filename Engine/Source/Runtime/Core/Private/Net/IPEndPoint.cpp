// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Net/IPEndPoint.h"

namespace Ayla
{
	IPEndPoint::IPEndPoint(const IPAddress& address, uint16 port)
		: m_Address(address)
		, m_Port(port)
	{
	}

	AddressFamily IPEndPoint::GetAddressFamily() const
	{
		return m_Address.GetAddressFamily();
	}

	String IPEndPoint::ToString() const
	{
		if (GetAddressFamily() == AddressFamily::InterNetwork)
		{
			return String::Format(TEXT("{}:{}"), m_Address.ToString(), m_Port);
		}
		else
		{
			return String::Format(TEXT("[{}]:{}"), m_Address.ToString(), m_Port);
		}
	}

	bool IPEndPoint::operator ==(const IPEndPoint& other) const noexcept
	{
		return m_Address == other.m_Address && m_Port == other.m_Port;
	}

	bool IPEndPoint::operator !=(const IPEndPoint& other) const noexcept
	{
		return !(*this == other);
	}

	IPEndPoint IPEndPoint::Any(uint16 port)
	{
		return IPEndPoint(IPAddress::Any(), port);
	}
}