// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <exception>
#include <format>
#include <string>
#include <string_view>
#include <source_location>

class invalid_operation : public std::exception
{
	std::string _message;
	std::source_location _source;
	std::string _what;

public:
	invalid_operation(std::string_view message, const std::source_location& source = std::source_location::current())
		: _message(message)
		, _source(source)
	{
		_what = std::format("{}\n  at {} in {}:{}", _message, source.function_name(), source.file_name(), source.line());
	}

	invalid_operation(std::wstring_view message, const std::source_location& source = std::source_location::current())
		: invalid_operation(String::Cast<std::string>(message), source)
	{
	}

	virtual const char* what() const noexcept override
	{
		return _what.c_str();
	}
};

class task_canceled : public std::exception
{
	std::string _message;
	std::source_location _source;
	std::string _what;

public:
	task_canceled(const std::source_location& source = std::source_location::current())
		: _message("Task was aborted.")
		, _source(source)
	{
		_what = std::format("{}\n  at {} in {}:{}", _message, source.function_name(), source.file_name(), source.line());
	}

	virtual const char* what() const noexcept override
	{
		return _what.c_str();
	}
};