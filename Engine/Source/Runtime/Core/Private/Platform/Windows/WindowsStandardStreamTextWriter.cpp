// Copyright 2020-2022 Aumoa.lib. All right reserved.

module;

#define __ALLOW_PLATFORM_COMMON_H__
#include "System/AssertionMacros.h"
#include "Platform/PlatformCommon.h"
#undef __ALLOW_PLATFORM_COMMON_H__

export module Core:WindowsStandardStreamTextWriter;

export import :TextWriter;
export import :String;

#if PLATFORM_WINDOWS

export class WindowsStandardStreamTextWriter : public TextWriter
{
	HANDLE hStd = NULL;

public:
	WindowsStandardStreamTextWriter(DWORD StdHandleId)
		: hStd(GetStdHandle(StdHandleId))
	{
	}

	virtual void Write(String Val) override
	{
		if (hStd)
		{
			DWORD Written = 0;
			auto Wide = Val.AsCodepage();
			WriteFile(hStd, Wide.c_str(), (DWORD)Wide.size(), &Written, NULL);
		}
	}

	HANDLE GetNativeHandle() const
	{
		return hStd;
	}
};

#endif