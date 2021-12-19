// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#undef GetObject

#include "Threading/Thread.h"
#include "Misc/StringUtils.h"
#include "GC/GC.h"

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
	
	if (StringUtils::Trim(FriendlyName).empty())
	{
		FriendlyName = std::to_wstring(ThreadId);
	}
}

Thread::~Thread()
{
	if (ThreadHandle)
	{
		::CloseHandle(ThreadHandle);
		ThreadHandle = nullptr;
	}
}

void Thread::SetFriendlyName(std::wstring_view InFriendlyName)
{
	SetThreadDescription(ThreadHandle, InFriendlyName.data());
	FriendlyName = InFriendlyName;
}

std::wstring Thread::GetFriendlyName()
{
	return FriendlyName;
}

int64 Thread::GetThreadId()
{
	return ThreadId;
}

bool Thread::IsManaged()
{
	return bIsManaged;
}

void Thread::SuspendThread()
{
	::SuspendThread(ThreadHandle);
}

void Thread::ResumeThread()
{
	::ResumeThread(ThreadHandle);
}

void Thread::Internal_NewThread(std::wstring FriendlyName, std::function<void()> Body)
{
	std::thread([Body, FriendlyName = std::move(FriendlyName)]()
	{
		Thread* MyThread = GetCurrentThread();
		MyThread->SetFriendlyName(FriendlyName);
		MyThread->bIsManaged = true;

		try
		{
			GC.RegisterThread(MyThread);
			Body();
		}
		catch (...)
		{
			GC.UnregisterThread(MyThread);
			throw;
		}

		GC.UnregisterThread(MyThread);
	}).detach();
}

Thread* Thread::GetCurrentThread()
{
	static thread_local Thread MyThread;
	return &MyThread;
}