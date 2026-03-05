// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Net/EndPoint.h"

namespace Ayla
{
	class CORE_API IPEndPoint : public EndPoint
	{
	private:
		IPAddress m_Address;
		uint16 m_Port;

	public:
		IPEndPoint(const IPAddress& address, uint16 port);

		const IPAddress& GetAddress() const noexcept { return m_Address; }
		uint16 GetPort() const noexcept { return m_Port; }

		void SetAddress(const IPAddress& address) noexcept { m_Address = address; }
		void SetPort(uint16 port) noexcept { m_Port = port; }

		AddressFamily GetAddressFamily() const override;
		String ToString() const override;

		bool operator ==(const IPEndPoint& other) const noexcept;
		bool operator !=(const IPEndPoint& other) const noexcept;

	public:
		static IPEndPoint Any(uint16 port);
	};
}