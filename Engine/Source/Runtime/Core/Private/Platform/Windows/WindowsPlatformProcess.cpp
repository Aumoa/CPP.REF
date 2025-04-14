// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/PlatformMacros.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/Windows/WindowsPlatformProcess.h"
#include "Platform/PlatformCommon.h"
#include "Console.h"
#include "Path.h"
#include "Diagnostics/StackFrame.h"
#include "Diagnostics/StackTrace.h"
#include "Diagnostics/Debug.h"
#include "WindowsStackTrace.h"
#include <mutex>
#include <map>

namespace Ayla
{
	struct WindowsPlatformProcess::SnapshotRunner
	{
		static std::unique_ptr<SnapshotRunner> Run()
		{
			auto i = std::make_unique<SnapshotRunner>();
			i->Start();
			return i;
		}

		Spinlock m_Mtx;
		std::map<DWORD, HANDLE> m_Handles;

	private:
		void Start()
		{
			using namespace std::chrono_literals;

			Spinlock mtx;
			SpinlockConditionVariable cv;
			bool init = false;
			std::thread([&]()
			{
				Capture();

				auto lock = std::unique_lock(mtx);
				cv.NotifyOne();
				init = true;
				lock.unlock();

				while (true)
				{
					std::this_thread::sleep_for(10s);
					Capture();
				}
			}).detach();

			auto lock = std::unique_lock(mtx);
			while (init == false)
			{
				cv.Wait(lock);
			}
		}

		void Capture()
		{
			HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
			if (hThreadSnapshot == INVALID_HANDLE_VALUE)
			{
				Debug::LogCritical(TEXT("LogPlatform"), TEXT("Failed to create snapshot of threads."));
				return;
			}

			DWORD currentProcessId = GetCurrentProcessId();
			DWORD currentThreadId = GetCurrentThreadId();

			THREADENTRY32 te32;
			te32.dwSize = sizeof(THREADENTRY32);

			std::vector<DWORD> query;
			if (Thread32First(hThreadSnapshot, &te32))
			{
				do
				{
					if (te32.th32OwnerProcessID == currentProcessId && te32.th32ThreadID != currentThreadId)
					{
						query.emplace_back(te32.th32ThreadID);
					}
				} while (Thread32Next(hThreadSnapshot, &te32));
			}
			else
			{
				Debug::LogCritical(TEXT("LogPlatform"), TEXT("Failed to retrieve thread information."));
			}

			auto lock = std::unique_lock(m_Mtx);
			std::map<DWORD, HANDLE> handlesCopy;
			std::swap(handlesCopy, m_Handles);
			for (auto& threadId : query)
			{
				auto it = handlesCopy.find(threadId);
				if (it == handlesCopy.end())
				{
					HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, threadId);
					m_Handles.emplace(threadId, hThread);
				}
				else
				{
					m_Handles.emplace(it->first, it->second);
					handlesCopy.erase(it);
				}
			}
			lock.unlock();

			for (auto& [_1, handle] : handlesCopy)
			{
				CloseHandle(handle);
			}

			CloseHandle(hThreadSnapshot);
			hThreadSnapshot = INVALID_HANDLE_VALUE;
		}
	};

	struct WindowsPlatformProcess::SuspendToken
	{
		std::vector<HANDLE> m_SuspendThreads;
	};

	bool WindowsPlatformProcess::IsDebuggerPresent() noexcept
	{
		return ::IsDebuggerPresent() == TRUE;
	}

	void WindowsPlatformProcess::OutputDebugString(String InMessage) noexcept
	{
		::OutputDebugStringW(InMessage.c_str());
	}

	void WindowsPlatformProcess::StacktraceCurrent(std::vector<StackFrame>& OutStackframes) noexcept
	{
		HANDLE hThread = GetCurrentThread();

		CONTEXT Context;
		RtlCaptureContext(&Context);

		OutStackframes = Capture(hThread, &Context);
	}

	void WindowsPlatformProcess::StacktraceFromThread(void* Handle, std::vector<StackFrame>& OutStackframes) noexcept
	{
		HANDLE hThread = (HANDLE)Handle;
		const bool bSameThread = GetThreadId(hThread) == GetThreadId(GetCurrentThread());
		if (bSameThread)
		{
			StacktraceCurrent(OutStackframes);
			return;
		}

		SuspendThread(hThread);

		CONTEXT Context;
		GetThreadContext(hThread, &Context);

		OutStackframes = Capture(hThread, &Context);

		ResumeThread(hThread);
	}

	void WindowsPlatformProcess::StacktraceFromException(void* lpExceptionPointers, std::vector<StackFrame>& OutStackframes) noexcept
	{
		OutStackframes = Capture(GetCurrentThread(), ((LPEXCEPTION_POINTERS)lpExceptionPointers)->ContextRecord);
	}

	extern "C" LONG CALLBACK UnhandledExceptionTrace(LPEXCEPTION_POINTERS lpExceptionPointer)
	{
		Console::Error.WriteLine(StackTrace::FromException(lpExceptionPointer).ToString());
		return EXCEPTION_CONTINUE_SEARCH;
	}

	void WindowsPlatformProcess::SetupStacktraceSignals() noexcept
	{
		SetUnhandledExceptionFilter(UnhandledExceptionTrace);
	}

	String WindowsPlatformProcess::FindEngineDirectory()
	{
		// Find 'Core.dll' library.
		HMODULE hModule = GetModuleHandleW(TEXT("Core.dll").c_str());
		if (hModule == nullptr)
		{
			return String::GetEmpty();
		}

		TCHAR Buf[1024];
		DWORD Len = GetModuleFileNameW(hModule, Buf, 1024);
		if (Len == 0)
		{
			return String::GetEmpty();
		}

		// Get engine directory from relative path of 'Core.dll'.
		String FileName = String(Buf, Len);
		// Engine/Binaries/{Platform}/{Configuration}/
		String BinariesDirectory = Path::GetDirectoryName(FileName);
		// Engine/
		String EngineDirectory = Path::Combine(BinariesDirectory, TEXT(".."), TEXT(".."), TEXT(".."));
		EngineDirectory = Path::GetFullPath(EngineDirectory);

		return EngineDirectory;
	}

	void* WindowsPlatformProcess::AllocateCurrentThreadHandle() noexcept
	{
		HANDLE hCurrentProcess = GetCurrentProcess();
		HANDLE hCurrentThread = GetCurrentThread();
		DuplicateHandle(hCurrentProcess, hCurrentThread, hCurrentProcess, &hCurrentThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
		return hCurrentThread;
	}

	void WindowsPlatformProcess::DestroyCurrentThreadHandle(void* InHandle) noexcept
	{
		CloseHandle((HANDLE)InHandle);
	}

	void WindowsPlatformProcess::SetThreadDescription(void* InHandle, String InDescription) noexcept
	{
		::SetThreadDescription((HANDLE)InHandle, (LPCWSTR)InDescription.c_str());
	}

	void* WindowsPlatformProcess::CreateProcess(const ProcessStartInfo& InStartInfo) noexcept
	{
		STARTUPINFOW StartupInfo = {};
		PROCESS_INFORMATION ProcessInfo = {};

		BOOL bStatus = ::CreateProcessW(
			InStartInfo.FileName.c_str(),
			(LPWSTR)InStartInfo.Arguments.c_str(),
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&StartupInfo,
			&ProcessInfo
		);
		check(bStatus);

		if (bStatus)
		{
			CloseHandle(ProcessInfo.hThread);
			ProcessInfo.hThread = NULL;
		}

		return ProcessInfo.hProcess;
	}

	void WindowsPlatformProcess::CloseProcessHandle(void* InHandle) noexcept
	{
		BOOL bStatus = CloseHandle((HANDLE)InHandle);
		check(bStatus);
	}

	bool WindowsPlatformProcess::SetEnvironmentVariable(String InName, String InValue) noexcept
	{
		return ::SetEnvironmentVariableW(InName.c_str(), InValue.c_str()) == TRUE;
	}

	String WindowsPlatformProcess::GetEnvironmentVariable(String InName) noexcept
	{
		static thread_local WCHAR Buf[32767];
		DWORD Len = ::GetEnvironmentVariableW(InName.c_str(), Buf, (DWORD)AE_ARRAYSIZE(Buf));
		if (Len == 0)
		{
			return String::GetEmpty();
		}
		return String::FromLiteral(std::wstring_view(Buf, (size_t)Len));
	}

	auto WindowsPlatformProcess::SuspendAllThreads() noexcept -> SuspendToken*
	{
		static auto s_Snapshot = SnapshotRunner::Run();

		auto token = new SuspendToken();
		auto currentId = GetCurrentThreadId();
		auto lock = std::unique_lock(s_Snapshot->m_Mtx);
		for (auto& [id, handle] : s_Snapshot->m_Handles)
		{
			if (id != currentId)
			{
				token->m_SuspendThreads.emplace_back(handle);
			}
		}
		lock.unlock();

		for (auto& handle : token->m_SuspendThreads)
		{
			SuspendThread(handle);
		}

		return token;
	}

	void WindowsPlatformProcess::ResumeAllThreads(SuspendToken* token) noexcept
	{
		if (token != nullptr)
		{
			for (auto& hThread : token->m_SuspendThreads)
			{
				ResumeThread(hThread);
			}

			delete token;
		}
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif