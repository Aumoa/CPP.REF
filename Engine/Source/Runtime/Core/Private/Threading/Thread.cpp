// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Threading/Thread.h"
#include "Misc/PlatformMacros.h"
#include "Misc/String.h"

using namespace libty;

#if PLATFORM_WINDOWS
#include <Windows.h>
#undef GetObject
#endif

Thread::Thread()
	: ThreadId(std::this_thread::get_id())
{
	ThreadId = std::this_thread::get_id();

#if PLATFORM_WINDOWS
	HANDLE CurrentThread = ::GetCurrentThread();
	int64 tid = ::GetThreadId(CurrentThread);
	ThreadHandle = ::OpenThread(GENERIC_ALL, TRUE, (DWORD)tid);

	PWSTR pwThreadDesc = nullptr;
	::GetThreadDescription(CurrentThread, &pwThreadDesc);

	if (pwThreadDesc)
	{
		FriendlyName = pwThreadDesc;
	}

	if (String::Trim(FriendlyName).empty())
	{
		FriendlyName = std::to_wstring(tid);
	}
#endif

	SToken = new ThreadSuspendToken(this);
	JoinSource = TaskCompletionSource<>::Create();
}

Thread::~Thread()
{
#if PLATFORM_WINDOWS
	if (ThreadHandle)
	{
		::CloseHandle(ThreadHandle);
		ThreadHandle = nullptr;
	}
#endif

	delete SToken;
}

void Thread::SetFriendlyName(std::wstring_view InFriendlyName)
{
#if PLATFORM_WINDOWS
	SetThreadDescription(ThreadHandle, InFriendlyName.data());
#endif
	FriendlyName = InFriendlyName;
}

void Thread::SuspendThread()
{
	std::unique_lock lock(SuspendMtx);
	SuspendCv.wait(lock);
}

void Thread::ResumeThread()
{
	std::unique_lock lock(SuspendMtx);
	SuspendCv.notify_all();
}

void Thread::Join()
{
	JoinSource.GetTask().Wait();
}

Task<> Thread::JoinAsync()
{
	return JoinSource.GetTask();
}

std::wstring Thread::GetFriendlyName() const
{
	return FriendlyName;
}

std::thread::id Thread::GetThreadId() const
{
	return ThreadId;
}

bool Thread::IsManaged() const
{
	return bIsManaged;
}

auto Thread::GetSuspendToken() const -> ThreadSuspendToken*
{
	return SToken;
}

Thread* Thread::CreateThread(std::wstring_view FriendlyName, std::function<void()> ThreadEntry)
{
	std::promise<Thread*> ThreadPtr;
	std::future<Thread*> ThreadPtrFuture = ThreadPtr.get_future();

	std::thread([ThreadEntry, FriendlyName = std::wstring(FriendlyName), &ThreadPtr]()
	{
		Thread* MyThread = GetCurrentThread();
		MyThread->SetFriendlyName(FriendlyName);
		MyThread->bIsManaged = true;

		ThreadPtr.set_value(MyThread);
		try
		{
			ThreadEntry();
		}
		catch (...)
		{
			MyThread->JoinSource.SetException(std::current_exception());
			return;
		}

		MyThread->JoinSource.SetResult();
	}).detach();

	return ThreadPtrFuture.get();
}

Thread* Thread::GetCurrentThread()
{
	static thread_local Thread MyThread;
	return &MyThread;
}