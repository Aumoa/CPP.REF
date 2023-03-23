// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreTypes/String.h"
#include "Diagnostics/Stacktrace.h"
#include "Threading/Spinlock.h"
#include <exception>
#include <string>
#include <string_view>
#include <set>

class CORE_API Exception
{
	String _message;
	std::exception_ptr _innerException;
	Stacktrace _stacktrace;

public:
	Exception(const String& message = TEXT("An exception was thrown."), std::exception_ptr innerException = {}) noexcept;
	Exception(const Exception& rhs) noexcept;
	Exception(Exception&& rhs) noexcept;
	virtual ~Exception() noexcept;

	virtual String ToString() const noexcept;

	virtual const Stacktrace& GetStacktrace() const noexcept;
	virtual String GetMessage() const noexcept;
	virtual std::exception_ptr GetInnerException() const noexcept;

	static bool TryGetExceptionFromPointer(std::exception_ptr Pointer, std::function<void(const Exception&)> Catch);

public:
	void InternalMarkStacktrace(void*) noexcept;

private:
	static Spinlock sLock;
	static std::set<Exception*> sMarks;

	static void MarkExceptionPointer(Exception* ptr) noexcept;
	static void UnmarkExceptionPointer(Exception* ptr) noexcept;
	
public:
	static Exception* EnsureException(Exception* ptr) noexcept;
};