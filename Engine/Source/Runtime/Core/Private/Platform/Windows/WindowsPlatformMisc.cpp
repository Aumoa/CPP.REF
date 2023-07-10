// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsCore.h"

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



bool PlatformMisc::SetThreadName(String name) noexcept
{
	return SetThreadDescription(GetCurrentThread(), name.c_str()) == S_OK;
}

String PlatformMisc::GetThreadName() noexcept
{
	WCHAR* Buf;
	if (FAILED(GetThreadDescription(GetCurrentThread(), &Buf)))
	{
		return TEXT("");
	}

	auto Str = String(Buf);
	LocalFree(Buf);
	return Str;
}

size_t PlatformMisc::GetCurrentThreadId() noexcept
{
	return (size_t)GetThreadId(GetCurrentThread());
}



void PlatformMisc::Debugbreak() noexcept
{
	__debugbreak();
}

void PlatformMisc::OutputDebugString(String InMessage) noexcept
{
	OutputDebugStringW(InMessage.c_str());
}

bool PlatformMisc::IsDebuggerPresent() noexcept
{
	return ::IsDebuggerPresent() == TRUE;
}

void PlatformMisc::Abort() noexcept
{
	::abort();
}



void PlatformMisc::UuidCreate(void* block128) noexcept
{
	if (::UuidCreate((UUID*)block128) != RPC_S_OK)
	{
		__noop;
	}
}

String PlatformMisc::FormatSystemCode(int32 InSystemCode)
{
	static thread_local WCHAR Buf[1024] = {};
	DWORD Len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, (DWORD)InSystemCode, 0, Buf, 1024, NULL);
	if (Len == 0)
	{
		return TEXT("Unknown error.");
	}

	return String(Buf, Len);
}

int32 PlatformMisc::GetLastError() noexcept
{
	return (int32)::GetLastError();
}

String PlatformMisc::GetSystemPath()
{
	static thread_local WCHAR Buf[1024] = {};
	::GetSystemDirectoryW(Buf, 1024);
	return String(Buf);
}



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