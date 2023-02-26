// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Console.h"
#include <mutex>

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#include "Threading/Spinlock.h"

inline namespace
{
	CONSOLE_SCREEN_BUFFER_INFO sScreenInfo;
	HANDLE sStdout;
	Spinlock sLock;
}
#endif

void Console::Write(String message)
{
	static int _trap = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	std::unique_lock lock(sLock);
	DWORD written = 0;
	WriteConsoleW(sStdout, message.c_str(), (DWORD)message.length(), &written, NULL);
#endif
}

void Console::WriteLine(String message)
{
	static int _trap = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	std::unique_lock lock(sLock);
	static thread_local wchar_t sBuf[2048];
	size_t length = message.length();
	memcpy(sBuf, message.c_str(), sizeof(wchar_t) * length);
	sBuf[length++] = L'\n';

	DWORD written = 0;
	WriteConsoleW(sStdout, sBuf, (DWORD)length, &written, NULL);
#endif
}

void Console::SetForegroundColor(EConsoleColor color)
{
	static int _trp = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	std::unique_lock lock(sLock);
	sScreenInfo.wAttributes = (sScreenInfo.wAttributes & 0xFF00) | (WORD)color;
	SetConsoleTextAttribute(sStdout, sScreenInfo.wAttributes);
#endif
}

EConsoleColor Console::GetForegroundColor()
{
	static int _trp = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	std::unique_lock lock(sLock);
	return (EConsoleColor)(sScreenInfo.wAttributes & 0x00FF);
#endif
}

void Console::_trap_init()
{
#if PLATFORM_WINDOWS
	static int _trap = []()
	{
		sStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(sStdout, &sScreenInfo);
		return 0;
	}();
#endif
}