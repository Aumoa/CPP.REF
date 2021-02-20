// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "InvalidArgumentException.h"

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