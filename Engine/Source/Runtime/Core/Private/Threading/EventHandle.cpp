// Copyright 2020-2021 Aumoa.lib. All right reserved.

#define NOMINMAX

#include <Windows.h>
#undef GetObject

#include "Threading/EventHandle.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

using enum ELogVerbosity;

SEventHandle::SEventHandle() : Super()
{
	_handle = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
	if (_handle == nullptr)
	{
		LogSystem::Log(LogCore, Error, L"Could not create event handle. GetLastError(): {}", GetLastError());
		return;
	}
}

SEventHandle::~SEventHandle()
{
	if (_handle != nullptr)
	{
		CloseHandle(_handle);
		_handle = nullptr;
	}
}

auto SEventHandle::Wait(std::chrono::milliseconds timeout) -> EStatus
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

void SEventHandle::Set()
{
	SetEvent(_handle);
}

void SEventHandle::Reset()
{
	ResetEvent(_handle);
}