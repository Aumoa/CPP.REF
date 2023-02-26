// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Exceptions/Exception.h"
#include "CoreTypes/String.h"
#include <mutex>

Exception::Exception(const String& message, std::exception_ptr innerException) noexcept
	: _message(message)
	, _innerException(innerException)
{
	MarkExceptionPointer(this);
}

Exception::Exception(const Exception& rhs) noexcept
	: _message(rhs._message)
	, _innerException(rhs._innerException)
	, _stacktrace(rhs._stacktrace)
{
	MarkExceptionPointer(this);
}

Exception::Exception(Exception&& rhs) noexcept
	: _message(std::move(rhs._message))
	, _innerException(std::move(rhs._innerException))
	, _stacktrace(std::move(rhs._stacktrace))
{
	MarkExceptionPointer(this);
}

Exception::~Exception() noexcept
{
	UnmarkExceptionPointer(this);
}

String Exception::ToString() const noexcept
{
	if (_innerException)
	{
		String ss = String::Format(TEXT("{}: {}\n"), String(typeid(*this).name()), _message);
		try
		{
			std::rethrow_exception(_innerException);
		}
		catch (const Exception& e)
		{
			ss += String::Format(TEXT("---> {}\n"), e);
		}
		catch (const std::exception& e)
		{
			ss += String::Format(TEXT("---> {}: {}\n"), String(typeid(e).name()), String(e.what()));
		}
		catch (...)
		{
			ss += String::Format(TEXT("---> Unknown exception.\n"));
		}
		ss += String::Format(TEXT("--- End of inner exception stack trace ---\n{}"), _stacktrace);
		return ss;
	}
	else
	{
		return String::Format(TEXT("{}: {}\n{}"), String(typeid(*this).name()), _message, _stacktrace);
	}
}

const Stacktrace& Exception::GetStacktrace() const noexcept
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

void Exception::InternalMarkStacktrace(void* lpExceptionPointers) noexcept
{
	_stacktrace = Stacktrace::FromException(lpExceptionPointers);
}

Spinlock Exception::sLock;
std::set<Exception*> Exception::sMarks;

void Exception::MarkExceptionPointer(Exception* ptr) noexcept
{
	std::unique_lock lock(sLock);
	sMarks.emplace(ptr);
}

void Exception::UnmarkExceptionPointer(Exception* ptr) noexcept
{
	std::unique_lock lock(sLock);
	sMarks.erase(ptr);
}

Exception* Exception::EnsureException(Exception* ptr) noexcept
{
	std::unique_lock lock(sLock);
	auto it = sMarks.find(ptr);
	if (it == sMarks.end())
	{
		return nullptr;
	}
	return *it;
}