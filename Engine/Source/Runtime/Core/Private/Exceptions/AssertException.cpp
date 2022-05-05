// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/AssertException.h"
#include "Misc/String.h"

using namespace libty;

AssertException::AssertException(StringView exp, const std::source_location& src)
	: FatalException(String::Format(TEXT("Assertion failed: !({})"), exp), nullptr, src)
{
}

AssertException::AssertException(StringView exp, StringView msg, const std::source_location& src)
	: FatalException(String::Format(TEXT("Assertion failed: !({})\n{}"), exp, msg), nullptr, src)
{
}