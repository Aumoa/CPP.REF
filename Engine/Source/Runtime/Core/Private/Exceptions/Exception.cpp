// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Exceptions/Exception.h"

GENERATE_BODY(SException);

SException::SException() : This(L"An unexpected error occurred.")
{
}

SException::SException(std::wstring_view Message, SException* InnerException) : Super()
	, Message(Message)
	, InnerException(InnerException)
{
}

std::wstring SException::ToString()
{
	return std::format(L"{}: {}", GetType()->GetFullName(), Message);
}