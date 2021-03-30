// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "InvalidCastException.h"

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