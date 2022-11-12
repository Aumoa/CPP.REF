// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include <exception>
#include <string>
#include <string_view>
#include <stacktrace>

class CORE_API Exception
{
	struct _impl_buf
	{
		bool _cached;
		String _description;
		String _stacktrace;
		String _fulltrace;
	};

	std::stacktrace _stacktrace;
	String _message;
	std::exception_ptr _innerException;
	std::shared_ptr<_impl_buf> _impl;

public:
	Exception(const String& message = TEXT("An exception was thrown."), std::exception_ptr innerException = {}) noexcept;
	Exception(const Exception& rhs) noexcept;
	Exception(Exception&& rhs) noexcept;
	virtual ~Exception() noexcept;

	virtual String ToString() const noexcept;

	virtual const std::stacktrace& GetStacktrace() const noexcept;
	virtual String GetMessage() const noexcept;
	virtual std::exception_ptr GetInnerException() const noexcept;

private:
	void _cache_string() const noexcept;
	static String _stacktrace_to_string(const std::stacktrace& st) noexcept;
};