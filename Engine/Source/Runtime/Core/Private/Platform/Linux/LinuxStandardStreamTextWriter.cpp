// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxStandardStreamTextWriter.h"

#if PLATFORM_LINUX

#include <iostream>

LinuxStandardStreamTextWriter::LinuxStandardStreamTextWriter(int32 IO)
	: IO(IO)
{
}

void LinuxStandardStreamTextWriter::Write(String Val)
{
	switch (IO)
	{
	case 0:
		std::wcout << Val.wstring_view();
		break;
	case 1:
		std::wcerr << Val.wstring_view();
		break;
	}
}

#endif