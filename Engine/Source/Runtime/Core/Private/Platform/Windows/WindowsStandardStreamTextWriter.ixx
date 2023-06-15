// Copyright 2020-2022 Aumoa.lib. All right reserved.

module;

#include "Platform/Windows/WindowsCore.h"

export module Core.Platform.Windows:WindowsStandardStreamTextWriter;

export import Core.IO;

#if PLATFORM_WINDOWS

export class WindowsStandardStreamTextWriter : public TextWriter
{
	HANDLE hStd = NULL;

public:
	WindowsStandardStreamTextWriter(DWORD StdHandleId);

	virtual void Write(char_t Val) override;
	virtual void Write(String Val) override;

	HANDLE GetNativeHandle() const { return hStd; }
};

#endif