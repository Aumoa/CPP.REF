// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

//#include "CoreTypes/IntegralTypes.h"
#include "CoreTypes/String.h"

class CORE_API EndPoint
{
public:
	struct sockaddr_buf
	{
		uint8 _padding[44];
	};

public:
	virtual String ToString() const = 0;
	virtual void ApplyTo(sockaddr_buf& buf) const = 0;
	virtual size_t Size() const = 0;
	virtual std::unique_ptr<EndPoint> Clone() const = 0;
	virtual std::unique_ptr<EndPoint> Accept(const EndPoint::sockaddr_buf& ep) const = 0;
};