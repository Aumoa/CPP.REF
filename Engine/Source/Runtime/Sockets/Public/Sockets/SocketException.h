// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <exception>
#include <source_location>
#include <string>
#include <format>

class socket_exception : public std::exception
{
	std::string _message;
	std::source_location _location;
	std::string _what;

public:
	socket_exception(std::string_view message, const std::source_location& location = std::source_location::current())
		: _message(message)
		, _location(location)
	{
		_what = std::format("{}: {}\n  at {} in {}:{}", typeid(*this).name(), _message, location.function_name(), location.file_name(), location.line());
	}

	socket_exception(std::wstring_view message, const std::source_location& location = std::source_location::current())
		: socket_exception(String::AsMultibyte(message), location)
	{
	}

	virtual const char* what() const noexcept override
	{
		return _what.c_str();
	}
};