// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Exception.h"

#include <utility>
#include <typeinfo>

#undef interface
#include <Windows.h>

using namespace std;

Exception::Exception()
	: innerException(nullptr)
{
	message = L"Unexpected error.";

#ifdef _DEBUG
	OutputDebugStringW(String::Format(L"{0}: {1}\n", Name->C_Str, message->C_Str)->C_Str);
#endif
}

Exception::Exception(TRefPtr<String> message)
	: innerException(nullptr)
{
	this->message = message;

#ifdef _DEBUG
	OutputDebugStringW(String::Format(L"{0}: {1}\n", Name->C_Str, message->C_Str)->C_Str);
#endif
}

Exception::Exception(TRefPtr<String> message, Exception* innerException)
{
	this->message = message;
	this->innerException = innerException;

#ifdef _DEBUG
	OutputDebugStringW(String::Format(L"{0}: {1}\n", Name->C_Str, message->C_Str)->C_Str);
#endif
}

Exception::Exception(Exception&& other) noexcept
{
	message = move(other.message);
	innerException = other.innerException;
	copy_name = other.Name;

	other.innerException = nullptr;

#ifdef _DEBUG
	OutputDebugStringW(String::Format(L"{0}: {1}\n", Name->C_Str, message->C_Str)->C_Str);
#endif
}

Exception::~Exception()
{
	if (innerException != nullptr)
	{
		delete innerException;
		innerException = nullptr;
	}
}

TRefPtr<String> Exception::Name_get() const
{
	if (copy_name.IsValid)
	{
		return copy_name;
	}
	else
	{
		return typeid(*this).name();
	}
}

TRefPtr<String> Exception::Message_get() const
{
	return message;
}

Exception* Exception::InnerException_get() const
{
	return innerException;
}