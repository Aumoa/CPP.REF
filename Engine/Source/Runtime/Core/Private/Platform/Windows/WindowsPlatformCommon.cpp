// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include <Windows.h>
#include <wrl/client.h>
#undef TEXT
#undef OutputDebugString
#include "System/LanguageSupportMacros.h"
#include "System/AssertionMacros.h"

export module Core:WindowsPlatformCommon;

export import :String;
export import :AssertionMacros;
export import :PlatformProcess;

export using Microsoft::WRL::ComPtr;

export namespace WindowsPlatformCommon
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

	[[noreturn]]
	inline void ReportHResult(HRESULT Result)
	{
		checkf(SUCCEEDED(Result), TEXT("Failed to execute D3D12 operation. Error: 0x{:08X}"), Result);
	}
}