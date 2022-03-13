// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/Exceptions.h"

class shader_compiler_exception : public std::exception
{
	std::string _api;
	std::string _message;
	std::source_location _location;
	std::string _what;

public:
	shader_compiler_exception(std::string_view API, std::string_view Message, const std::source_location& location = std::source_location::current())
		: _api(API)
		, _message(Message)
		, _location(location)
	{
		_what = std::format("{} in '{}' API\n  at {} in {}:{}", _message, _api, _location.function_name(), _location.file_name(), _location.line());
	}

	virtual const char* what() const noexcept override
	{
		return _what.c_str();
	}
};