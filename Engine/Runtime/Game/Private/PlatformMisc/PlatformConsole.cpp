// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "PlatformMisc/PlatformConsole.h"

#include "Windows/WindowsMinimal.h"
#include "Logging/LogMacros.h"

bool PlatformConsole::Alloc()
{
	return (bool)AllocConsole();
}

void PlatformConsole::Free()
{
	if (!FreeConsole())
	{
		SE_LOG(LogEngine, Warning, L"Failed to free platform console.");
	}
}

void PlatformConsole::Write(TRefPtr<String> message)
{
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message->C_Str, (DWORD)message->Length, nullptr, nullptr);
}

void PlatformConsole::WriteLine(TRefPtr<String> message)
{
	Write(String::Format(L"{0}\n", message));
}