// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS
#include "Platform/Windows/WindowsCore.h"

import Core.System;
import Core.Platform.Windows;

inline namespace
{
	WindowsStandardStreamTextWriter sOut(STD_OUTPUT_HANDLE);
	WindowsStandardStreamTextWriter sError(STD_ERROR_HANDLE);
	HANDLE hIn(GetStdHandle(STD_INPUT_HANDLE));
}

TextWriter& Console::GetOut()
{
	return sOut;
}

TextWriter& Console::GetError()
{
	return sError;
}
#endif

void Console::Write(String Str)
{
#if PLATFORM_WINDOWS
	sOut.Write(Str);
#endif
}

void Console::WriteLine(String Str)
{
#if PLATFORM_WINDOWS
	sOut.WriteLine(Str);
#endif
}

String Console::ReadLine()
{
#if PLATFORM_WINDOWS
	static thread_local std::vector<wchar_t> Records(256);

	size_t Seekpos = 0;
	DWORD EOL = static_cast<DWORD>(-1);
	size_t Buf = Records.size();

	while (true)
	{
		DWORD Reads = 0;
		if (ReadConsoleW(hIn, Records.data() + Seekpos, (DWORD)Buf, &Reads, NULL) == FALSE)
		{
			throw SystemException(GetLastError());
		}

		for (size_t I = 0; I < (size_t)Reads; ++I)
		{
			size_t Index = I + Seekpos;
			if (Records[Index] == '\r' || Records[Index] == '\n')
			{
				EOL = (DWORD)Index;
				break;
			}
		}

		if (EOL == (DWORD)-1)
		{
			Seekpos += Buf;
			Buf = Records.size();

			size_t Size = Seekpos + Buf;
			if (Records.size() < Size)
			{
				Records.resize(Size);
			}
		}
		else
		{
			break;
		}
	}

	return String(Records.data(), (size_t)EOL);
#endif
}

void Console::SetForegroundColor(EConsoleColor InColor)
{
#if PLATFORM_WINDOWS
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	if (GetConsoleScreenBufferInfo(sOut.GetNativeHandle(), &BufferInfo) == FALSE)
	{
		return;
	}
	BufferInfo.wAttributes = (BufferInfo.wAttributes & 0xFF00) | (WORD)InColor;
	if (SetConsoleTextAttribute(sOut.GetNativeHandle(), BufferInfo.wAttributes) == FALSE)
	{
		return;
	}
#endif
}

EConsoleColor Console::GetForegroundColor()
{
#if PLATFORM_WINDOWS
	CONSOLE_SCREEN_BUFFER_INFO BufferInfo;
	if (GetConsoleScreenBufferInfo(sOut.GetNativeHandle(), &BufferInfo) == FALSE)
	{
		return (EConsoleColor)-1;
	}
	return (EConsoleColor)(BufferInfo.wAttributes & 0x00FF);
#endif
}