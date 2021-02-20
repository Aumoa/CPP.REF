// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "FormatException.h"

FormatException::FormatException() : Super(L"Invalid format or argument detected.")
{

}

FormatException::FormatException(TRefPtr<String> message) : Super(message)
{

}

FormatException::FormatException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

FormatException::~FormatException()
{

}