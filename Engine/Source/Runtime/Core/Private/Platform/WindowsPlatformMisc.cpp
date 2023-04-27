// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#pragma pack(push, 8)

#include <Windows.h>
#include <minwindef.h>
#include <rpc.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Psapi.h>
#include <imagehlp.h>

#undef YieldProcessor
#undef OutputDebugString
#undef GetMessage
#undef GetCurrentDirectory
#undef SetCurrentDirectory

#pragma pack(pop)

#pragma pop_macro("TEXT")

import Core.System;
import Core.Platform;

void PlatformMisc::InitializeSpinlock(size_t& Lock) noexcept
{
	::InitializeSRWLock((PSRWLOCK)&Lock);
}

void PlatformMisc::DestroySpinlock(size_t& Lock) noexcept
{
	UNREFERENCED_PARAMETER(Lock);
	__noop;
}

void PlatformMisc::AcquireSpinlockExclusive(size_t& Lock) noexcept
{
	::AcquireSRWLockExclusive((PSRWLOCK)&Lock);
}

void PlatformMisc::ReleaseSpinlockExclusive(size_t& Lock) noexcept
{
	::ReleaseSRWLockExclusive((PSRWLOCK)&Lock);
}

bool PlatformMisc::TryAcquireSpinlockExclusive(size_t& Lock) noexcept
{
	return ::TryAcquireSRWLockExclusive((PSRWLOCK)&Lock) == TRUE;
}

void PlatformMisc::AcquireSpinlockShared(size_t& Lock) noexcept
{
	::AcquireSRWLockShared((PSRWLOCK)&Lock);
}

void PlatformMisc::ReleaseSpinlockShared(size_t& Lock) noexcept
{
	::ReleaseSRWLockShared((PSRWLOCK)&Lock);
}

bool PlatformMisc::TryAcquireSpinlockShared(size_t& Lock) noexcept
{
	return ::TryAcquireSRWLockShared((PSRWLOCK)&Lock) == TRUE;
}



void PlatformMisc::InitializeConditionVariable(size_t& Cv) noexcept
{
	::InitializeConditionVariable((PCONDITION_VARIABLE)&Cv);
}

void PlatformMisc::DestroyConditionVariable(size_t& Cv) noexcept
{
	UNREFERENCED_PARAMETER(Cv);
	__noop;
}

void PlatformMisc::NotifyOneConditionVariable(size_t& Cv) noexcept
{
	::WakeConditionVariable((PCONDITION_VARIABLE)&Cv);
}

void PlatformMisc::NotifyAllConditionVariable(size_t& Cv) noexcept
{
	::WakeAllConditionVariable((PCONDITION_VARIABLE)&Cv);
}

bool PlatformMisc::SleepConditionVariable(size_t& Cv, size_t& Lock, uint32 Delay, bool bShared) noexcept
{
	return ::SleepConditionVariableSRW((PCONDITION_VARIABLE)&Cv, (PSRWLOCK)&Lock, (DWORD)Delay, bShared ? CONDITION_VARIABLE_LOCKMODE_SHARED : 0) == TRUE;
}



//bool PlatformMisc::SetThreadName(String name) noexcept
//{
//	return SetThreadDescription(GetCurrentThread(), name.c_str()) == S_OK;
//}
//
//String PlatformMisc::GetThreadName() noexcept
//{
//	WCHAR* buf;
//	if (FAILED(GetThreadDescription(GetCurrentThread(), &buf)))
//	{
//		return TEXT("");
//	}
//
//	auto str = String(buf);
//	LocalFree(buf);
//	return str;
//}
//
//size_t PlatformMisc::GetCurrentThreadId() noexcept
//{
//	return (size_t)GetThreadId(GetCurrentThread());
//}

//class WindowsSEHException : public Exception
//{
//public:
//	WindowsSEHException(uint32 exceptionCode)
//		: Exception(String::Format(TEXT("An SEH exception occurred with exception code: {}"), exceptionCode))
//	{
//	}
//};
//
//static constexpr DWORD CxxExceptionCode = 0xE06D7363;
//
//LONG CALLBACK UnhandledExceptionHandler(LPEXCEPTION_POINTERS lpExceptionPointers)
//{
//	if (lpExceptionPointers->ExceptionRecord->ExceptionCode == CxxExceptionCode)
//	{
//		auto* ptr = Exception::EnsureException((Exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
//		if (ptr)
//		{
//			Log::Fatal(LogWindows, TEXT("{}"), *ptr);
//		}
//	}
//	else
//	{
//		Log::Fatal(LogWindows, TEXT("{}"), Stacktrace::FromException(lpExceptionPointers));
//	}
//
//	return EXCEPTION_CONTINUE_SEARCH;
//}
//
//LONG CALLBACK MarkExceptionStacktrace(LPEXCEPTION_POINTERS lpExceptionPointers)
//{
//	if (lpExceptionPointers->ExceptionRecord->ExceptionCode == CxxExceptionCode)
//	{
//		auto* ptr = Exception::EnsureException((Exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
//		if (ptr)
//		{
//			ptr->InternalMarkStacktrace(lpExceptionPointers);
//		}
//		else if (_is_exception_typeof(typeid(std::exception), lpExceptionPointers))
//		{
//			auto inner = std::make_exception_ptr((std::exception*)lpExceptionPointers->ExceptionRecord->ExceptionInformation[1]);
//			throw CxxException(inner);
//		}
//	}
//
//	return EXCEPTION_CONTINUE_SEARCH;
//}
//
//void CALLBACK SEHExceptionTranslator(unsigned int lExceptionCode, LPEXCEPTION_POINTERS lpExceptionPointers)
//{
//	LPEXCEPTION_RECORD lpExceptionRecord = lpExceptionPointers->ExceptionRecord;
//	DWORD exceptionCode = lpExceptionRecord->ExceptionCode;
//	switch (exceptionCode)
//	{
//	case EXCEPTION_ACCESS_VIOLATION:
//		throw AccessViolationException((AccessViolationException::EAccessMode)lpExceptionRecord->ExceptionInformation[0], (int64)lpExceptionRecord->ExceptionInformation[1]);
//	case EXCEPTION_STACK_OVERFLOW:
//		throw StackOverflowException();
//	}
//}
//
//int _internal_guarded_main(std::function<int()>* body)
//{
//	uint32 exceptionCode = 0;
//
//	_set_se_translator(SEHExceptionTranslator);
//
//	SetUnhandledExceptionFilter(UnhandledExceptionHandler);
//	AddVectoredExceptionHandler(1, MarkExceptionStacktrace);
//
//	return (*body)();
//}
//
//int PlatformMisc::GuardedMain(std::function<int()> body)
//{
//	return _internal_guarded_main(&body);
//}
//
//
//
//void PlatformMisc::Debugbreak() noexcept
//{
//	__debugbreak();
//}
//
//void PlatformMisc::OutputDebugString(String message) noexcept
//{
//	OutputDebugStringW(message.c_str());
//}
//
//
//
//void PlatformMisc::UuidCreate(void* block128) noexcept
//{
//	if (::UuidCreate((UUID*)block128) != RPC_S_OK)
//	{
//		__noop;
//	}
//}
//
//String PlatformMisc::FormatSystemCode(int32 systemCode)
//{
//	static thread_local WCHAR buf[1024] = {};
//	DWORD len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)systemCode, 0, buf, 1024, NULL);
//	if (len == 0)
//	{
//		return TEXT("Unknown error.");
//	}
//
//	return String(buf, len);
//}
//
//int32 PlatformMisc::GetLastError() noexcept
//{
//	return (int32)::GetLastError();
//}
//
//String PlatformMisc::GetSystemPath()
//{
//	static thread_local WCHAR buf[1024] = {};
//	::GetSystemDirectoryW(buf, 1024);
//	return String(buf);
//}



std::wstring PlatformMisc::FromCodepage(std::string_view Str, int32 Codepage) noexcept
{
	if (Str.length() == 0)
	{
		return L"";
	}

	int32 Length = MultiByteToWideChar(Codepage, 0, Str.data(), (int32)Str.length(), nullptr, 0);
	if (Length == 0)
	{
		return L"";
	}

	std::wstring Unicode;
	Unicode.resize(Length);
	MultiByteToWideChar(Codepage, 0, Str.data(), (int32)Str.length(), Unicode.data(), Length);

	return Unicode;
}

std::string PlatformMisc::AsCodepage(std::wstring_view Str, int32 Codepage) noexcept
{
	if (Str.length() == 0)
	{
		return "";
	}

	int32 Length = WideCharToMultiByte(Codepage, 0, Str.data(), (int32)Str.length(), nullptr, 0, nullptr, nullptr);
	if (Length == 0)
	{
		return "";
	}

	std::string Multibyte;
	Multibyte.resize(Length);
	WideCharToMultiByte(Codepage, 0, Str.data(), (int32)Str.length(), Multibyte.data(), Length, nullptr, nullptr);

	return Multibyte;
}

#endif