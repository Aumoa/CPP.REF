// Copyright 2020 Aumoa.lib. All right reserved.

#include "Exception.h"

#include <utility>
#include <typeinfo>

using namespace SC::Runtime::Core;

using namespace std;

Exception::Exception()
	: innerException(nullptr)
{
	message = L"Unexpected error.";
}

Exception::Exception(TRefPtr<String> message)
	: innerException(nullptr)
{
	this->message = message;
}

Exception::Exception(TRefPtr<String> message, Exception* innerException)
{
	this->message = message;
	this->innerException = innerException;
}

Exception::Exception(Exception&& other) noexcept
{
	message = move(other.message);
	innerException = other.innerException;
	copy_name = other.Name;

	other.innerException = nullptr;
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