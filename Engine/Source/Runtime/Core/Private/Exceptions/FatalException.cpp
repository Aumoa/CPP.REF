// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/FatalException.h"
#include "Misc/String.h"
#include "Diagnostics/LogSystem.h"
#include "Diagnostics/LogCategory.h"

DEFINE_LOG_CATEGORY(LogException);

using namespace libty;

thread_local std::vector<FatalException*> sTLExceptions;

FatalException::FatalException(StringView message, std::exception_ptr innerException) noexcept
	: _stacktrace(Stacktrace::CaptureCurrent())
	, _message(message)
	, _innerException(innerException)
	, _impl_buf(std::make_unique<_Impl_buf>())
{
	sTLExceptions.emplace_back(this);
}

FatalException::FatalException(const FatalException& rhs) noexcept
	: _stacktrace(rhs._stacktrace)
	, _message(rhs._message)
	, _innerException(rhs._innerException)
	, _impl_buf(rhs._impl_buf)
{
}

FatalException::FatalException(FatalException&& rhs) noexcept
	: _stacktrace(std::move(rhs._stacktrace))
	, _message(std::move(rhs._message))
	, _innerException(std::move(rhs._innerException))
	, _impl_buf(std::move(rhs._impl_buf))
{
}

FatalException::~FatalException() noexcept
{
	auto it = std::find(sTLExceptions.begin(), sTLExceptions.end(), this);
	if (it != sTLExceptions.end())
	{
		sTLExceptions.erase(it);
	}
}

const char* FatalException::what() const noexcept
{
	this->_Cache_strings();
	return _impl_buf->_what.c_str();
}

StringView FatalException::ToString() const noexcept
{
	this->_Cache_strings();
	return _impl_buf->_fulltrace;
}

const Stacktrace& FatalException::GetStacktrace() const noexcept
{
	return _stacktrace;
}

StringView FatalException::GetMessage() const noexcept
{
	return _message;
}

std::exception_ptr FatalException::GetInnerException() const noexcept
{
	return _innerException;
}

FatalException* FatalException::AvailableException() noexcept
{
	if (sTLExceptions.empty())
	{
		return nullptr;
	}

	return sTLExceptions.back();
}

void FatalException::_Cache_strings() const noexcept
{
	if (!_impl_buf->_cached)
	{
		// Unwinding descriptions.
		if (_innerException)
		{
			try
			{
				std::rethrow_exception(_innerException);
			}
			catch (const FatalException& fe)
			{
				fe._Cache_strings();
				_impl_buf->_description = fe._impl_buf->_description;
				_impl_buf->_stacktrace = fe._impl_buf->_stacktrace;
			}
			catch (...)
			{
			}
		}

		if (!_impl_buf->_description.empty())
		{
			_impl_buf->_description += TEXT("\n ---> ");
		}
		if (!_impl_buf->_stacktrace.empty())
		{
			_impl_buf->_stacktrace += TEXT("\n   --- End of inner exception stack trace ---\n");
		}

		_impl_buf->_description += String::Format(TEXT("{}: {}"), StringView(typeid(*this).name()), _message);
		_impl_buf->_stacktrace += _stacktrace.Trace();

		_impl_buf->_fulltrace = String::Format(TEXT("{}\n{}"), _impl_buf->_description, _impl_buf->_stacktrace);
		_impl_buf->_what = String::AsMultibyte(_impl_buf->_fulltrace);
		_impl_buf->_cached = true;
	}
}