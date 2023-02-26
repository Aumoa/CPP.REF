// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsPlatformMisc.h"

#if PLATFORM_WINDOWS

#include "Misc/PlatformMisc.h"
#include "CoreTypes/String.h"
#include "Exceptions/SystemException.h"
#include "Exceptions/ArgumentNullException.h"
#include "Exceptions/AccessViolationException.h"
#include "Exceptions/StackOverflowException.h"
#include "Exceptions/CxxException.h"
#include "IO/IOCompletionOverlapped.h"
#include "Logging/Log.h"
#include "Logging/LogCategory.h"

static constexpr LogCategory LogWindows(TEXT("LogWindows"));

void PlatformMisc::InitializeSpinlock(size_t& lock) noexcept
{
	::InitializeSRWLock((PSRWLOCK)&lock);
}

void PlatformMisc::DestroySpinlock(size_t& lock) noexcept
{
	__noop;
}

void PlatformMisc::AcquireSpinlockExclusive(size_t& lock) noexcept
{
	::AcquireSRWLockExclusive((PSRWLOCK)&lock);
}

void PlatformMisc::ReleaseSpinlockExclusive(size_t& lock) noexcept
{
	::ReleaseSRWLockExclusive((PSRWLOCK)&lock);
}

bool PlatformMisc::TryAcquireSpinlockExclusive(size_t& lock) noexcept
{
	return ::TryAcquireSRWLockExclusive((PSRWLOCK)&lock) == TRUE;
}

void PlatformMisc::AcquireSpinlockShared(size_t& lock) noexcept
{
	::AcquireSRWLockShared((PSRWLOCK)&lock);
}

void PlatformMisc::ReleaseSpinlockShared(size_t& lock) noexcept
{
	::ReleaseSRWLockShared((PSRWLOCK)&lock);
}

bool PlatformMisc::TryAcquireSpinlockShared(size_t& lock) noexcept
{
	return ::TryAcquireSRWLockShared((PSRWLOCK)&lock) == TRUE;
}



void PlatformMisc::InitializeConditionVariable(size_t& cv) noexcept
{
	::InitializeConditionVariable((PCONDITION_VARIABLE)&cv);
}

void PlatformMisc::DestroyConditionVariable(size_t& cv) noexcept
{
	__noop;
}

void PlatformMisc::NotifyOneConditionVariable(size_t& cv) noexcept
{
	::WakeConditionVariable((PCONDITION_VARIABLE)&cv);
}

void PlatformMisc::NotifyAllConditionVariable(size_t& cv) noexcept
{
	::WakeAllConditionVariable((PCONDITION_VARIABLE)&cv);
}

bool PlatformMisc::SleepConditionVariable(size_t& cv, size_t& lock, uint32 delay, bool shared) noexcept
{
	return ::SleepConditionVariableSRW((PCONDITION_VARIABLE)&cv, (PSRWLOCK)&lock, (DWORD)delay, shared ? CONDITION_VARIABLE_LOCKMODE_SHARED : 0) == TRUE;
}



bool PlatformMisc::SetThreadName(String name) noexcept
{
	return SetThreadDescription(GetCurrentThread(), name.c_str()) == S_OK;
}

String PlatformMisc::GetThreadName() noexcept
{
	WCHAR* buf;
	if (FAILED(GetThreadDescription(GetCurrentThread(), &buf)))
	{
		return TEXT("");
	}

	auto str = String(buf);
	LocalFree(buf);
	return str;
}

size_t PlatformMisc::GetCurrentThreadId() noexcept
{
	return (size_t)GetThreadId(GetCurrentThread());
}

class WindowsSEHException : public Exception
{
public:
	WindowsSEHException(uint32 exceptionCode)
		: Exception(String::Format(TEXT("An SEH exception occurred with exception code: {}"), exceptionCode))
	{
	}
};

static constexpr DWORD CxxExceptionCode = 0xE06D7363;

LONG CALLBACK UnhandledExceptionHandler(LPEXCEPTION_POINTERS lpExceptionPointers)
{
	if (lpExceptionPointers->ExceptionRecord->ExceptionCode == CxxExceptionCode)
	{
		auto* ptr = Exception::EnsureException((Exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
		if (ptr)
		{
			Log::Fatal(LogWindows, TEXT("{}"), *ptr);
		}
	}
	else
	{
		Log::Fatal(LogWindows, TEXT("{}"), Stacktrace::FromException(lpExceptionPointers));
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

LONG CALLBACK MarkExceptionStacktrace(LPEXCEPTION_POINTERS lpExceptionPointers)
{
	if (lpExceptionPointers->ExceptionRecord->ExceptionCode == CxxExceptionCode)
	{
		auto* ptr = Exception::EnsureException((Exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
		if (ptr)
		{
			ptr->InternalMarkStacktrace(lpExceptionPointers);
		}
		else if (_is_exception_typeof(typeid(std::exception), lpExceptionPointers))
		{
			auto inner = std::make_exception_ptr((std::exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
			throw CxxException(inner);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

void CALLBACK SEHExceptionTranslator(unsigned int lExceptionCode, LPEXCEPTION_POINTERS lpExceptionPointers)
{
	LPEXCEPTION_RECORD lpExceptionRecord = lpExceptionPointers->ExceptionRecord;
	DWORD exceptionCode = lpExceptionRecord->ExceptionCode;
	switch (exceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		throw AccessViolationException((AccessViolationException::EAccessMode)lpExceptionRecord->ExceptionInformation[0], (int64)lpExceptionRecord->ExceptionInformation[1]);
	case EXCEPTION_STACK_OVERFLOW:
		throw StackOverflowException();
	}
}

int _internal_guarded_main(std::function<int()>* body)
{
	uint32 exceptionCode = 0;

	_set_se_translator(SEHExceptionTranslator);

	SetUnhandledExceptionFilter(UnhandledExceptionHandler);
	AddVectoredExceptionHandler(1, MarkExceptionStacktrace);

	return (*body)();
}

int PlatformMisc::GuardedMain(std::function<int()> body)
{
	return _internal_guarded_main(&body);
}



void PlatformMisc::Debugbreak() noexcept
{
	__debugbreak();
}

void PlatformMisc::OutputDebugString(String message) noexcept
{
	OutputDebugStringW(message.c_str());
}



void PlatformMisc::UuidCreate(void* block128) noexcept
{
	if (::UuidCreate((UUID*)block128) != RPC_S_OK)
	{
		__noop;
	}
}

String PlatformMisc::FormatSystemCode(int32 systemCode)
{
	static thread_local WCHAR buf[1024] = {};
	DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)systemCode, 0, buf, 1024, NULL);
	if (len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(buf, len);
}

int32 PlatformMisc::GetLastError() noexcept
{
	return (int32)::GetLastError();
}

String PlatformMisc::GetSystemPath()
{
	static thread_local WCHAR buf[1024] = {};
	::GetSystemDirectoryW(buf, 1024);
	return String(buf);
}



std::wstring PlatformMisc::FromCodepage(std::string_view s, int32 codepage) noexcept
{
	if (s.length() == 0)
	{
		return L"";
	}

	int32 length = MultiByteToWideChar(codepage, 0, s.data(), (int32)s.length(), nullptr, 0);
	if (length == 0)
	{
		return L"";
	}

	std::wstring unicode;
	unicode.resize(length);
	MultiByteToWideChar(codepage, 0, s.data(), (int32)s.length(), unicode.data(), length);

	return unicode;
}

std::string PlatformMisc::AsCodepage(std::wstring_view s, int32 codepage) noexcept
{
	if (s.length() == 0)
	{
		return "";
	}

	int32 length = WideCharToMultiByte(codepage, 0, s.data(), (int32)s.length(), nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		return "";
	}

	std::string multibyte;
	multibyte.resize(length);
	WideCharToMultiByte(codepage, 0, s.data(), (int32)s.length(), multibyte.data(), length, nullptr, nullptr);

	return multibyte;
}

#endif