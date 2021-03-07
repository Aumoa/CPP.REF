// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Windows/WinEvent.h"

#include "Logging/LogMacros.h"

using namespace std;

WinEvent::WinEvent(HANDLE hEvent) : Super()
{
	Handle = hEvent;
}

WinEvent::WinEvent(bool bManual, bool bInitialSet, TRefPtr<String> eventName) : Super()
{
	DWORD flags = 0;
	if (bManual) flags |= CREATE_EVENT_MANUAL_RESET;
	if (bInitialSet) flags |= CREATE_EVENT_INITIAL_SET;

	Handle = CreateEventExW(nullptr, nullptr, flags, GENERIC_ALL);
}

WinEvent::~WinEvent()
{

}

bool WinEvent::Wait(optional<Milliseconds> timeout)
{
	DWORD time = timeout.has_value() ? (DWORD)timeout.value().Value : INFINITE;
	return WaitForSingleObject(Handle, time) == WAIT_OBJECT_0;
}

TRefPtr<WinEvent> WinEvent::OpenEvent(TRefPtr<String> eventName)
{
	if (!eventName.IsValid)
	{
		throw InvalidArgumentException(L"eventName is nullptr.");
	}

	HANDLE hOpened = ::OpenEventW(GENERIC_ALL, FALSE, eventName->C_Str);
	if (hOpened == INVALID_HANDLE_VALUE)
	{
		SE_LOG(LogWindows, Error, L"Event name({0}) is not a valid name.", eventName);
		return nullptr;
	}

	return NewObject<WinEvent>(hOpened);
}