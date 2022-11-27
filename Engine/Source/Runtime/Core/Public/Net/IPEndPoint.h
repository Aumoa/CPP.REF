// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Net/EndPoint.h"
#include "Net/IPAddress.h"

class CORE_API IPEndPoint : public EndPoint
{
	IPAddress _addr;
	uint16 _port = 0;

public:
	IPEndPoint() = default;
	IPEndPoint(const IPEndPoint&) = default;
	IPEndPoint(IPEndPoint&&) noexcept = default;

	IPEndPoint(const IPAddress& ipaddr, uint16 port);

	virtual String ToString() const override;
	virtual void ApplyTo(sockaddr_buf& buf) const override;
	virtual size_t Size() const override;
	virtual std::unique_ptr<EndPoint> Clone() const override;
	virtual std::unique_ptr<EndPoint> Accept(const EndPoint::sockaddr_buf& ep) const override;

	void Accept(const EndPoint::sockaddr_buf& ep);

	static IPEndPoint Any();
};