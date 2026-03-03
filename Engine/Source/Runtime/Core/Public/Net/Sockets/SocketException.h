// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Exception.h"
#include "Net/Sockets/SocketError.h"

namespace Ayla
{
	class CORE_API SocketException : public Exception
	{
	public:
		SocketException(SocketError error);
	};
}