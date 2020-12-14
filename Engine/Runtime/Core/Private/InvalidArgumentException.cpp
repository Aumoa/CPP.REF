// Copyright 2020 Aumoa.lib. All right reserved.

#include "InvalidArgumentException.h"

using namespace SC::Runtime::Core;

InvalidArgumentException::InvalidArgumentException() : Super(L"Invalid argument detected.")
{

}

InvalidArgumentException::InvalidArgumentException(TRefPtr<String> message) : Super(message)
{

}

InvalidArgumentException::InvalidArgumentException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

InvalidArgumentException::~InvalidArgumentException()
{

}