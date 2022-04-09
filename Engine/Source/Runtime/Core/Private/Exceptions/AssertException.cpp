// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/AssertException.h"
#include "Misc/String.h"

using namespace libty;

AssertException::AssertException(std::string_view exp, const std::source_location& src)
	: FatalException(String::Format("Assertion failed: !({})", exp), nullptr, src)
{
}

AssertException::AssertException(std::string_view exp, std::string_view msg, const std::source_location& src)
	: FatalException(String::Format("Assertion failed: !({})\n{}", exp, msg), nullptr, src)
{
}

AssertException::AssertException(std::string_view exp, std::wstring_view msg, const std::source_location& src)
	: FatalException(String::Format("Assertion failed: !({})\n{}", exp, String::AsMultibyte(msg)), nullptr, src)
{
}