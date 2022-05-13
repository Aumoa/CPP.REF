// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/AssertException.h"
#include "Misc/String.h"

using namespace libty;

AssertException::AssertException(StringView exp)
	: Exception(String::Format(TEXT("Assertion failed: !({})"), exp))
{
}

AssertException::AssertException(StringView exp, StringView msg)
	: Exception(String::Format(TEXT("Assertion failed: !({})\n{}"), exp, msg))
{
}