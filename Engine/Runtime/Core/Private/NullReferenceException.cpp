// Copyright 2020 Aumoa.lib. All right reserved.

#include "NullReferenceException.h"

using namespace SC::Runtime::Core;

NullReferenceException::NullReferenceException() : Super(L"Null reference detected.")
{

}

NullReferenceException::NullReferenceException(TRefPtr<String> message) : Super(message)
{

}

NullReferenceException::NullReferenceException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

NullReferenceException::~NullReferenceException()
{

}