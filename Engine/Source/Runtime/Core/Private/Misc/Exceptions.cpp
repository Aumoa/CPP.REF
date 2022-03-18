// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/Exceptions.h"
#include "Misc/String.h"

invalid_operation::invalid_operation(std::string_view message, const std::source_location& source)
	: _message(message)
	, _source(source)
{
	_what = String::Format("{}\n  at {} in {}:{}", _message, source.function_name(), source.file_name(), source.line());
}

invalid_operation::invalid_operation(std::wstring_view message, const std::source_location& source)
	: invalid_operation(String::Cast<std::string>(message), source)
{
}

const char* invalid_operation::what() const noexcept
{
	return _what.c_str();
}

task_canceled::task_canceled(const std::source_location& source)
	: _message("Task was aborted.")
	, _source(source)
{
	_what = String::Format("{}\n  at {} in {}:{}", _message, source.function_name(), source.file_name(), source.line());
}

const char* task_canceled::what() const noexcept
{
	return _what.c_str();
}