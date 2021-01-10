// Copyright 2020 Aumoa.lib. All right reserved.

#include "EngineCriticalException.h"

EngineCriticalException::EngineCriticalException(TRefPtr<String> title, TRefPtr<String> message) : Super(message)
{

}

EngineCriticalException::~EngineCriticalException()
{

}

TRefPtr<String> EngineCriticalException::GetTitle() const
{
	return title;
}

TRefPtr<String> EngineCriticalException::GetMessage() const
{
	return message;
}