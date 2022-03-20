// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/Exceptions.h"
#include "Misc/String.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"

DEFINE_LOG_CATEGORY(LogException);

fatal_exception::fatal_exception(std::string_view message, const std::source_location& src)
	: _message(message)
	, _src(src)
{
	_what = String::Format("{}\n  at {} in {}:{}", _message, _src.function_name(), _src.file_name(), _src.line());
	SE_LOG(LogException, Error, String::AsUnicode(_what));
}

fatal_exception::fatal_exception(std::wstring_view message, const std::source_location& src)
	: fatal_exception(String::AsMultibyte(message), src)
{
}

const char* fatal_exception::what() const noexcept
{
	return _what.c_str();
}

const std::source_location& fatal_exception::source() const noexcept
{
	return _src;
}

std::string_view fatal_exception::message() const noexcept
{
	return _message;
}

assert_exception::assert_exception(std::string_view exp, const std::source_location& src)
	: fatal_exception(String::Format("Assertion failed: !({})", exp), src)
{
}

assert_exception::assert_exception(std::string_view exp, std::string_view msg, const std::source_location& src)
	: fatal_exception(String::Format("Assertion failed: !({})\n{}", exp, msg), src)
{
}

assert_exception::assert_exception(std::string_view exp, std::wstring_view msg, const std::source_location& src)
	: fatal_exception(String::Format("Assertion failed: !({})\n{}", exp, String::AsMultibyte(msg)), src)
{
}