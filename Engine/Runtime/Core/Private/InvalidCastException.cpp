// Copyright 2020 Aumoa.lib. All right reserved.

#include "InvalidCastException.h"

using namespace SC::Runtime::Core;

InvalidCastException::InvalidCastException() : Super(L"Invalid cast detected.")
{

}

InvalidCastException::InvalidCastException(TRefPtr<String> message) : Super(message)
{

}

InvalidCastException::InvalidCastException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

InvalidCastException::~InvalidCastException()
{

}