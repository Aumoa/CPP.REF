// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "Misc/String.h"

class CORE_API EndPoint
{
public:
	struct sockaddr_buf
	{
		uint8 _padding[28];
	};

public:
	virtual String ToString() const = 0;
	virtual void ApplyTo(sockaddr_buf& buf) const = 0;
	virtual size_t Size() const = 0;
};