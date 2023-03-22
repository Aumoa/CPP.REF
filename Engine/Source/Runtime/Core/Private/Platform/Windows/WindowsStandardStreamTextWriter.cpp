// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsStandardStreamTextWriter.h"
#include "CoreAssert.h"

WindowsStandardStreamTextWriter::WindowsStandardStreamTextWriter(DWORD StdHandleId)
	: hStd(GetStdHandle(StdHandleId))
{
	check(hStd);
}

void WindowsStandardStreamTextWriter::Write(char_t Val)
{
	DWORD Written = 0;
	WriteConsoleW(hStd, &Val, 1, &Written, NULL);
}

void WindowsStandardStreamTextWriter::Write(String Val)
{
	DWORD Written = 0;
	WriteConsoleW(hStd, Val.c_str(), (DWORD)Val.length(), &Written, NULL);
}

#endif