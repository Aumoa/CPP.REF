// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Sockets/SocketException.h"

GENERATE_BODY(SSocketException);

SSocketException::SSocketException(std::wstring_view Message, SException* InnerException) : Super(Message, InnerException)
{
}