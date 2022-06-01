// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Misc/String.h"
#include "Stacktrace/Stacktrace.h"
#include <exception>
#include <string>
#include <string_view>
#include <source_location>

class CORE_API Exception
{
	struct _Impl_buf
	{
		bool _cached;
		String _description;
		String _stacktrace;
		String _fulltrace;
	};

	Stacktrace _stacktrace;
	String _message;
	std::exception_ptr _innerException;
	std::shared_ptr<_Impl_buf> _impl_buf;

public:
	Exception(const String& message = TEXT("An exception was thrown."), std::exception_ptr innerException = {}) noexcept;
	Exception(const Exception& rhs) noexcept;
	Exception(Exception&& rhs) noexcept;
	virtual ~Exception() noexcept;

	virtual String ToString() const noexcept;

	virtual const Stacktrace& GetStacktrace() const noexcept;
	virtual String GetMessage() const noexcept;
	virtual std::exception_ptr GetInnerException() const noexcept;

	static Exception* AvailableException() noexcept;

private:
	void _Cache_strings() const noexcept;
};