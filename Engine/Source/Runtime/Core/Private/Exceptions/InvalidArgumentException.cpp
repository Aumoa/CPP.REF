// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/InvalidArgumentException.h"

GENERATE_BODY(SInvalidArgumentException);

SInvalidArgumentException::SInvalidArgumentException()
	: Super(L"An invalid argument was specified.")
{
}

SInvalidArgumentException::SInvalidArgumentException(std::wstring_view Message, SException* InnerException)
	: Super(Message, InnerException)
{
}