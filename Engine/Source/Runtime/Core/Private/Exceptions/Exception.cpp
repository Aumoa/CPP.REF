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
	std::wstring WstrMsg = std::format(L"{}: {}", GetType()->GetFullName(), Message);
	if (InnerException)
	{
		WstrMsg += std::format(L"  InnerException: {}", InnerException->ToString());
	}
	return WstrMsg;
}

std::wstring SException::GetName()
{
	return GetType()->GetFullName();
}

std::wstring SException::GetMessage()
{
	return Message;
}

SException* SException::GetInnerException()
{
	return InnerException;
}