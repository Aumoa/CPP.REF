// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IO/TextWriter.h"

#if PLATFORM_WINDOWS

#include "Platform/Windows/WindowsCore.h"

class WindowsStandardStreamTextWriter : public TextWriter
{
	HANDLE hStd = NULL;

public:
	WindowsStandardStreamTextWriter(DWORD StdHandleId);

	virtual void Write(char_t Val) override;
	virtual void Write(String Val) override;
};

#endif