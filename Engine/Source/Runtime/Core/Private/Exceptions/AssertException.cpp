// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/AssertException.h"
#include "Misc/String.h"

AssertException::AssertException(const String& exp)
	: Exception(String::Format(TEXT("Assertion failed: !({})"), exp))
{
}

AssertException::AssertException(const String& exp, const String& msg)
	: Exception(String::Format(TEXT("Assertion failed: !({})\n{}"), exp, msg))
{
}