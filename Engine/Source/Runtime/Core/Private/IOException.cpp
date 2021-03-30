// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "IOException.h"

IOException::IOException() : Super(L"Invalid argument detected.")
{

}

IOException::IOException(TRefPtr<String> message) : Super(message)
{

}

IOException::IOException(TRefPtr<String> message, Exception* innerException) : Super(message, innerException)
{

}

IOException::~IOException()
{

}