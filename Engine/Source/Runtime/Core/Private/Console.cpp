// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Console.h"
#include <mutex>

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#include "Threading/Spinlock.h"
#include "Platform/Windows/WindowsStandardStreamTextWriter.h"

inline namespace
{
	CONSOLE_SCREEN_BUFFER_INFO sScreenInfo;
	HANDLE sStdout;

	WindowsStandardStreamTextWriter sOut(STD_OUTPUT_HANDLE);
	WindowsStandardStreamTextWriter sError(STD_ERROR_HANDLE);
}

TextWriter& Console::Out = sOut;
TextWriter& Console::Error = sError;
#endif

void Console::Write(String Str)
{
	static int _trap = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	Out.Write(Str);
#endif
}

void Console::WriteLine(String Str)
{
	static int _trap = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	Out.WriteLine(Str);
#endif
}

void Console::SetForegroundColor(EConsoleColor color)
{
	static int _trp = (_trap_init(), 0);

#if PLATFORM_WINDOWS
	sScreenInfo.wAttributes = (sScreenInfo.wAttributes & 0xFF00) | (WORD)color;
	SetConsoleTextAttribute(sStdout, sScreenInfo.wAttributes);
#endif
}

EConsoleColor Console::GetForegroundColor()
{
	static int _trp = (_trap_init(), 0);

#if PLATFORM_WINDOWS
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