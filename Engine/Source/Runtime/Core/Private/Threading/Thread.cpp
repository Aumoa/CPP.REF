// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include "Threading/Thread.h"

SThread::SThread() : Super()
{
	_handle = ::GetCurrentThread();
	_threadId = ::GetThreadId(_handle);
}

SThread::~SThread()
{
}

void SThread::SetFriendlyName(std::wstring_view friendlyName)
{
	SetThreadDescription(_handle, friendlyName.data());
	_friendlyName = friendlyName;
}

std::wstring SThread::GetFriendlyName() const
{
	return _friendlyName;
}

int64 SThread::GetThreadId() const
{
	return _threadId;
}

SThread* SThread::GetCurrentThread()
{
	static thread_local SThread MyThread;
	return &MyThread;
}