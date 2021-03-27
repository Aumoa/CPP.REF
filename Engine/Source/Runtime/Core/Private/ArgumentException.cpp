// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "ArgumentException.h"

ArgumentException::ArgumentException() : Super(L"Invalid argument detected.")
{

}

ArgumentException::ArgumentException(TRefPtr<String> message) : Super(message)
{

}

ArgumentException::ArgumentException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

ArgumentException::~ArgumentException()
{

}