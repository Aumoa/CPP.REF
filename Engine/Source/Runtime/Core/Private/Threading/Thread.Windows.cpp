// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include <Windows.h>
#include "Threading/Thread.h"
#include "Misc/String.h"
#undef GetObject

Thread::Thread()
{
	HANDLE CurrentThread = ::GetCurrentThread();
	ThreadId = ::GetThreadId(CurrentThread);
	ThreadHandle = ::OpenThread(GENERIC_ALL, TRUE, (DWORD)ThreadId);

	PWSTR pwThreadDesc = nullptr;
	::GetThreadDescription(CurrentThread, &pwThreadDesc);

	if (pwThreadDesc)
	{
		FriendlyName = pwThreadDesc;
	}

	if (String::Trim(FriendlyName).empty())
	{
		FriendlyName = std::to_wstring(ThreadId);
	}

	SToken = new ThreadSuspendToken(this);
	JoinFuture = JoinPromise.get_future();
}

Thread::~Thread()
{
	if (ThreadHandle)
	{
		::CloseHandle(ThreadHandle);
		ThreadHandle = nullptr;
	}

	delete SToken;
}

void Thread::SetFriendlyName(std::wstring_view InFriendlyName)
{
	SetThreadDescription(ThreadHandle, InFriendlyName.data());
	FriendlyName = InFriendlyName;
}

void Thread::SuspendThread()
{
	::SuspendThread(ThreadHandle);
}

void Thread::ResumeThread()
{
	::ResumeThread(ThreadHandle);
}

void Thread::Join()
{
	JoinFuture.get();
}

std::wstring Thread::GetFriendlyName() const
{
	return FriendlyName;
}

int64 Thread::GetThreadId() const
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
		ThreadEntry();
		MyThread->JoinPromise.set_value();
	}).detach();

	return ThreadPtrFuture.get();
}

Thread* Thread::GetCurrentThread()
{
	static thread_local Thread MyThread;
	return &MyThread;
}

#endif