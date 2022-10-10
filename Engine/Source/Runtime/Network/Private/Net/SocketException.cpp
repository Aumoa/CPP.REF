// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Net/SocketException.h"

SocketException::SocketException(const String& message, std::exception_ptr innerException)
	: Super(message, innerException)
{
}