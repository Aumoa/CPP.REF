// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/PlatformMacros.h"
#include "Net/IPAddress.h"
#include "String_.h"

namespace Ayla
{
	class CORE_API EndPoint
	{
	public:
		virtual ~EndPoint() = default;
		virtual AddressFamily GetAddressFamily() const = 0;
		virtual String ToString() const = 0;
	};
}