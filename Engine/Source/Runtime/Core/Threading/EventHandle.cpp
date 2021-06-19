// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EventHandle.h"
#include <Windows.h>
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"
#include "LogCategory/LogCore.h"

using namespace std;
using namespace std::chrono;

using enum ELogVerbosity;

EventHandle::EventHandle() : Super()
{
	_handle = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
	if (_handle == nullptr)
	{
		LogSystem::Log(LogCore, Error, L"Could not create event handle. GetLastError(): {}", GetLastError());
		return;
	}
}

EventHandle::~EventHandle()
{
	if (_handle != nullptr)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}

auto EventHandle::Wait(milliseconds timeout) -> EStatus
{
	DWORD status = WaitForSingleObject((HANDLE)_handle, (DWORD)timeout.count());
	switch (status)
	{
	case WAIT_OBJECT_0:
		return EStatus::Succeeded;
	case WAIT_TIMEOUT:
		return EStatus::Timeout;
	default:
		return EStatus::Failed;
	}
}

void EventHandle::Set()
{
	SetEvent(_handle);
}

void EventHandle::Reset()
{
	ResetEvent(_handle);
}