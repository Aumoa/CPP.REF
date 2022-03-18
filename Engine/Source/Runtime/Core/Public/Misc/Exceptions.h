// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <source_location>

class CORE_API invalid_operation : public std::exception
{
	std::string _message;
	std::source_location _source;
	std::string _what;

public:
	invalid_operation(std::string_view message, const std::source_location& source = std::source_location::current());
	invalid_operation(std::wstring_view message, const std::source_location& source = std::source_location::current());

	virtual const char* what() const noexcept override;
};

class CORE_API task_canceled : public std::exception
{
	std::string _message;
	std::source_location _source;
	std::string _what;

public:
	task_canceled(const std::source_location& source = std::source_location::current());

	virtual const char* what() const noexcept override;
};