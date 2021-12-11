// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#undef GetObject

#include "Threading/Thread.h"

Thread::Thread()
{
	_handle = ::GetCurrentThread();
	_threadId = ::GetThreadId(_handle);
}

void Thread::SetFriendlyName(std::wstring_view friendlyName)
{
	SetThreadDescription(_handle, friendlyName.data());
	_friendlyName = friendlyName;
}

std::wstring Thread::GetFriendlyName() const
{
	return _friendlyName;
}

int64 Thread::GetThreadId() const
{
	return _threadId;
}

Thread* Thread::GetCurrentThread()
{
	static thread_local Thread MyThread;
	return &MyThread;
}