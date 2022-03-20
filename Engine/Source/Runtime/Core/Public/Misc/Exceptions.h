// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <exception>
#include <string>
#include <string_view>
#include <source_location>

class CORE_API fatal_exception : public std::exception
{
	std::string _message;
	std::source_location _src;
	std::string _what;

public:
	fatal_exception(std::string_view message, const std::source_location& src = std::source_location::current());
	fatal_exception(std::wstring_view message, const std::source_location& src = std::source_location::current());

	virtual const char* what() const noexcept override;
	virtual const std::source_location& source() const noexcept;
	virtual std::string_view message() const noexcept;
};

class invalid_operation : public fatal_exception
{
public:
	invalid_operation(const std::source_location& src = std::source_location::current())
		: fatal_exception("Invalid operation detected.", src)
	{
	}

	invalid_operation(std::string_view message, const std::source_location& src = std::source_location::current())
		: fatal_exception(message, src)
	{
	}

	invalid_operation(std::wstring_view message, const std::source_location& src = std::source_location::current())
		: fatal_exception(message, src)
	{
	}
};

class task_canceled : public fatal_exception
{
public:
	task_canceled(const std::source_location& src = std::source_location::current())
		: fatal_exception("Task was aborted.", src)
	{
	}
};

class CORE_API assert_exception : public fatal_exception
{
public:
	assert_exception(std::string_view exp, const std::source_location& src = std::source_location::current());
	assert_exception(std::string_view exp, std::string_view msg, const std::source_location& src = std::source_location::current());
	assert_exception(std::string_view exp, std::wstring_view msg, const std::source_location& src = std::source_location::current());
};