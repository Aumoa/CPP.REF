// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#pragma push_macro("NOMINMAX")

#undef TEXT
#undef NOMINMAX

#include <Windows.h>
#include <comdef.h>
#include <gdiplus.h>

#pragma pop_macro("TEXT")
#pragma pop_macro("NOMINMAX")

namespace WindowsTools
{
	inline String GetErrorText(DWORD dwError)
	{
		static thread_local WCHAR Buf[1024] = {};
		DWORD Len = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, Buf, 1024, NULL);
		if (Len == 0)
		{
			return TEXT("Unknown error.");
		}

		return String(Buf, Len);
	}

	inline String GetErrorText()
	{
		return GetErrorText(::GetLastError());
	}
}

#endif