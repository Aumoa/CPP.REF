// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/Exception.h"
#include "Misc/String.h"

Exception::Exception(const String& message, std::exception_ptr innerException) noexcept
	: _stacktrace(std::stacktrace::current(1))
	, _message(message)
	, _innerException(innerException)
	, _impl(std::make_unique<_impl_buf>())
{
}

Exception::Exception(const Exception& rhs) noexcept
	: _stacktrace(rhs._stacktrace)
	, _message(rhs._message)
	, _innerException(rhs._innerException)
	, _impl(rhs._impl)
{
}

Exception::Exception(Exception&& rhs) noexcept
	: _stacktrace(std::move(rhs._stacktrace))
	, _message(std::move(rhs._message))
	, _innerException(std::move(rhs._innerException))
	, _impl(std::move(rhs._impl))
{
}

Exception::~Exception() noexcept
{
}

String Exception::ToString() const noexcept
{
	this->_cache_string();
	return _impl->_fulltrace;
}

const std::stacktrace& Exception::GetStacktrace() const noexcept
{
	return _stacktrace;
}

String Exception::GetMessage() const noexcept
{
	return _message;
}

std::exception_ptr Exception::GetInnerException() const noexcept
{
	return _innerException;
}

void Exception::_cache_string() const noexcept
{
	if (!_impl->_cached)
	{
		// Unwinding descriptions.
		if (_innerException)
		{
			try
			{
				std::rethrow_exception(_innerException);
			}
			catch (const Exception& fe)
			{
				fe._cache_string();
				_impl->_description = fe._impl->_description;
				_impl->_stacktrace = fe._impl->_stacktrace;
			}
			catch (...)
			{
			}
		}

		if (_impl->_description)
		{
			_impl->_description += TEXT("\n ---> ");
		}
		if (_impl->_stacktrace)
		{
			_impl->_stacktrace += TEXT("\n   --- End of inner exception stack trace ---\n");
		}

		_impl->_description += String::Format(TEXT("{}: {}"), String(typeid(*this).name()), _message);
		_impl->_stacktrace += _stacktrace_to_string(_stacktrace);

		_impl->_fulltrace = String::Format(TEXT("{}\n{}"), _impl->_description, _impl->_stacktrace);
		_impl->_cached = true;
	}
}

String Exception::_stacktrace_to_string(const std::stacktrace& st) noexcept
{
	return String::FromCodepage(std::to_string(st), 0);
}