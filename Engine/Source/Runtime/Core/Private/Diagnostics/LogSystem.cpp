// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"
#include "Diagnostics/LogVerbosity.h"
#include "Misc/String.h"

class fatal_exception : public std::exception
{
	std::string _message;
	std::source_location _location;
	std::string _what;

public:
	fatal_exception(std::wstring_view message, const std::source_location& location = std::source_location::current())
		: _message(String::AsMultibyte(message))
		, _location(location)
	{
		_what = String::Format("{}\n  at {} in {}:{}", _message, _location.function_name(), _location.file_name(), _location.line());
	}

	virtual const char* what() const noexcept override
	{
		return _what.c_str();
	}
};

void LogSystem::InternalLog(LogCategory& Category, ELogVerbosity LogVerbosity, std::wstring& Message, const std::source_location& Location)
{
	Category.OnLog(LogVerbosity, Message);

	if (LogVerbosity == ELogVerbosity::Fatal)
	{
		throw fatal_exception(Message, Location);
	}
}