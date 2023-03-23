// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Console.h"

#if PLATFORM_WINDOWS
#include "Misc/WindowsPlatformMisc.h"
#include "Platform/Windows/WindowsStandardStreamTextWriter.h"
#include "Exceptions/NotSupportedException.h"

inline namespace
{
	WindowsStandardStreamTextWriter sOut(STD_OUTPUT_HANDLE);
	WindowsStandardStreamTextWriter sError(STD_ERROR_HANDLE);
}

TextWriter& Console::Out = sOut;
TextWriter& Console::Error = sError;
#endif

void Console::Write(String Str)
{
#if PLATFORM_WINDOWS
	Out.Write(Str);
#endif
}

void Console::WriteLine(String Str)
{
#if PLATFORM_WINDOWS
	Out.WriteLine(Str);
#endif
}

void Console::SetForegroundColor(EConsoleColor InColor)
{
#if PLATFORM_WINDOWS
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	if (GetConsoleScreenBufferInfo(sOut.GetNativeHandle(), &BufferInfo) == FALSE)
	{
		throw NotSupportedException();
	}
	BufferInfo.wAttributes = (BufferInfo.wAttributes & 0xFF00) | (WORD)InColor;
	if (SetConsoleTextAttribute(sOut.GetNativeHandle(), BufferInfo.wAttributes) == FALSE)
	{
		throw NotSupportedException();
	}
#endif
}

EConsoleColor Console::GetForegroundColor()
{
#if PLATFORM_WINDOWS
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	if (GetConsoleScreenBufferInfo(sOut.GetNativeHandle(), &BufferInfo) == FALSE)
	{
		throw NotSupportedException();
	}
	return (EConsoleColor)(BufferInfo.wAttributes & 0x00FF);
#endif
}