// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/FatalException.h"
#include "Misc/String.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"

DEFINE_LOG_CATEGORY(LogException);

using namespace libty;

FatalException::FatalException(StringView message, std::exception_ptr innerException, const std::source_location& src)
	: _message(message)
	, _src(src)
	, _innerException(innerException)
{
	_what = String::Format(TEXT("{}\n  at {} in {}:{}"), _message, StringView(_src.function_name()), StringView(_src.file_name()), _src.line());

	_what_mb = String::AsMultibyte(_what);
}

const char* FatalException::what() const noexcept
{
	return _what_mb.c_str();
}

const std::source_location& FatalException::GetSource() const noexcept
{
	return _src;
}

StringView FatalException::GetMessage() const noexcept
{
	return _message;
}

std::exception_ptr FatalException::GetInnerException() const noexcept
{
	return _innerException;
}