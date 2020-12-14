// Copyright 2020 Aumoa.lib. All right reserved.

#include "ArgumentNullException.h"

using namespace SC::Runtime::Core;

ArgumentNullException::ArgumentNullException() : Super(L"Null argument detected.")
{

}

ArgumentNullException::ArgumentNullException(TRefPtr<String> message) : Super(message)
{

}

ArgumentNullException::ArgumentNullException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

ArgumentNullException::~ArgumentNullException()
{

}