// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "WindowsPlatformMisc.h"

#if PLATFORM_WINDOWS

#include "Misc/PlatformMisc.h"
#include "Misc/String.h"
#include "Exceptions/SystemException.h"
#include "Exceptions/ArgumentNullException.h"
#include "IO/IOCompletionOverlapped.h"

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
		: Exception(TEXTF("An SEH exception occurred with exception code: {}", exceptionCode))
	{
	}
};

int _internal_guarded_main(std::function<int()>* body)
{
	uint32 exceptionCode = 0;

	__try
	{
		return (*body)();
	}
	__except ((exceptionCode = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER))
	{
		throw WindowsSEHException(exceptionCode);
	}
}

int PlatformMisc::GuardedMain(std::function<int()> body)
{
	return _internal_guarded_main(&body);
}



void PlatformMisc::Debugbreak() noexcept
{
	__debugbreak();
}

void PlatformMisc::YieldProcessor() noexcept
{
	_mm_pause();
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

String PlatformMisc::FormatSystemCode(int32 systemCode) noexcept
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



void PlatformMisc::InitializeIOCompletionPort(size_t* iocp)
{
	if (iocp == nullptr)
	{
		throw ArgumentNullException(TEXT("iocp"));
	}

	HANDLE hIOCP = NULL;
	hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (hIOCP == NULL)
	{
		throw SystemException(GetLastError());
	}

	*iocp = reinterpret_cast<size_t>(hIOCP);
}

void PlatformMisc::BindIOCompletionPort(size_t* iocp, void* nativeHandle)
{
	if (iocp == nullptr)
	{
		throw ArgumentNullException(TEXT("iocp"));
	}

	HANDLE hIOCP = reinterpret_cast<HANDLE>(*iocp);
	hIOCP = CreateIoCompletionPort(nativeHandle, hIOCP, reinterpret_cast<ULONG_PTR>(nativeHandle), 0);
	if (hIOCP == NULL)
	{
		throw SystemException(GetLastError());
	}

	*iocp = reinterpret_cast<size_t>(hIOCP);
}

void PlatformMisc::DestroyIOCompletionPort(size_t& iocp) noexcept
{
}

IOCompletionOverlapped* PlatformMisc::GetQueuedCompletionStatus(size_t& iocp, uint32 timeout, size_t* pTransferred) noexcept
{
	HANDLE hIOCP = reinterpret_cast<HANDLE>(iocp);
	DWORD transferred = 0;
	ULONG_PTR key = 0;
	OVERLAPPED* lpOverlapped = NULL;
	BOOL status = ::GetQueuedCompletionStatus(hIOCP, &transferred, &key, &lpOverlapped, timeout);

	if (pTransferred)
	{
		*pTransferred = (size_t)transferred;
	}

	if (lpOverlapped)
	{
		auto* ptr = IOCompletionOverlapped::FromOverlapped(lpOverlapped);
		return ptr;
	}

	return nullptr;
}

bool PlatformMisc::PostQueuedCompletionStatus(size_t& iocp, IOCompletionOverlapped* overlapped, size_t resolved) noexcept
{
	HANDLE hIOCP = reinterpret_cast<HANDLE>(iocp);
	return ::PostQueuedCompletionStatus(hIOCP, (DWORD)resolved, (ULONG_PTR)overlapped, reinterpret_cast<LPOVERLAPPED>(overlapped->ToOverlapped())) == TRUE;
}



void PlatformMisc::CreateAsyncFile(size_t* handle, String filename, EFileAccessMode accessMode, EFileSharedMode sharedMode)
{
	HANDLE hFile = CreateFileW(filename.c_str(), (DWORD)accessMode, (DWORD)sharedMode, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		throw SystemException(GetLastError());
	}

	*handle = reinterpret_cast<size_t>(hFile);
}

void PlatformMisc::CloseAsyncFile(size_t handle) noexcept
{
	CloseHandle(reinterpret_cast<HANDLE>(handle));
}

void PlatformMisc::WriteAsyncFile(size_t handle, std::span<const uint8> buf, IOCompletionOverlapped* overlapped)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(handle);
	if (::WriteFile(hFile, buf.data(), (DWORD)buf.size_bytes(), NULL, reinterpret_cast<LPOVERLAPPED>(overlapped->ToOverlapped())) == FALSE)
	{
		DWORD err = GetLastError();
		if (err != ERROR_IO_PENDING)
		{
			auto ptr = std::make_exception_ptr(SystemException(err));
			if (overlapped->Failed(ptr))
			{
				delete overlapped;
			}

			std::rethrow_exception(ptr);
		}
	}
}

void PlatformMisc::ReadAsyncFile(size_t handle, std::span<uint8> buf, IOCompletionOverlapped* overlapped)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(handle);
	if (::ReadFile(hFile, buf.data(), (DWORD)buf.size_bytes(), NULL, reinterpret_cast<LPOVERLAPPED>(overlapped->ToOverlapped())) == FALSE)
	{
		DWORD err = GetLastError();
		if (err != ERROR_IO_PENDING)
		{
			auto ptr = std::make_exception_ptr(SystemException(err));
			if (overlapped->Failed(ptr))
			{
				delete overlapped;
			}

			std::rethrow_exception(ptr);
		}
	}
}

size_t PlatformMisc::GetAsyncFileSize(size_t handle)
{
	HANDLE hFile = reinterpret_cast<HANDLE>(handle);
	DWORD dwSizeHigh = 0;
	DWORD dwSize = ::GetFileSize(hFile, &dwSizeHigh);

	if (dwSize == 0xFFFFFFFF)
	{
		throw SystemException(GetLastError());
	}

	return ((size_t)dwSizeHigh << 32) | (size_t)dwSize;
}

#endif