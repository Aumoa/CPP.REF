// Copyright 2020-2022 Aumoa.lib. All right reserved.

#if PLATFORM_WINDOWS

#include "WindowsStandardStreamTextWriter.h"
#include "System/AssertionMacros.h"

WindowsStandardStreamTextWriter::WindowsStandardStreamTextWriter(DWORD StdHandleId)
	: hStd(GetStdHandle(StdHandleId))
{
}

void WindowsStandardStreamTextWriter::Write(String Val)
{
	if (hStd)
	{
		DWORD Written = 0;
		auto Wide = Val.AsCodepage();
		WriteFile(hStd, Wide.c_str(), (DWORD)Wide.size(), &Written, NULL);
	}
}

#endif