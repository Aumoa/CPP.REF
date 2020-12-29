// Copyright 2020 Aumoa.lib. All right reserved.

#include "IndexOutOfRangeException.h"

IndexOutOfRangeException::IndexOutOfRangeException() : Super(L"Index out of range.")
{

}

IndexOutOfRangeException::IndexOutOfRangeException(TRefPtr<String> message) : Super(message)
{

}

IndexOutOfRangeException::IndexOutOfRangeException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

IndexOutOfRangeException::~IndexOutOfRangeException()
{

}