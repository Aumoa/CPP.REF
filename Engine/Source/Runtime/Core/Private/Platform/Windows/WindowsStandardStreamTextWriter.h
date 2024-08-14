// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include "IO/TextWriter.h"

class WindowsStandardStreamTextWriter : public TextWriter
{
	HANDLE hStd = NULL;

public:
	WindowsStandardStreamTextWriter(DWORD StdHandleId);

	virtual void Write(String Val) override;

	HANDLE GetNativeHandle() const { return hStd; }
};

#undef __ALLOW_PLATFORM_COMMON_H__

#endif