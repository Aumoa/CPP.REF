// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sockets/SocketException.h"

using namespace ::libty;
using namespace ::libty::Sockets;

SocketException::SocketException(std::string_view message, std::exception_ptr innerException, std::source_location location)
	: FatalException(message, innerException, location)
{
}