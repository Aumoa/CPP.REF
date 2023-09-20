// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/Generic/GenericPlatformCommon.h"

#if PLATFORM_WINDOWS

#include "System/String.h"
#include "System/AssertionMacros.h"

#pragma push_macro("TEXT")
#pragma push_macro("NOMINMAX")

#undef TEXT
#undef NOMINMAX

#include <Windows.h>
#include <minwindef.h>
#include <rpc.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Psapi.h>
#include <imagehlp.h>
#include <signal.h>
#include <comdef.h>
#include <gdiplus.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <dxcapi.h>

#undef YieldProcessor
#undef OutputDebugString
#undef GetMessage
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef Yield
#undef min
#undef max
#undef CreateProcess
#undef InterlockedIncrement
#undef InterlockedDecrement
#undef byte
#undef FormatMessage
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable

#pragma pop_macro("TEXT")
#pragma pop_macro("NOMINMAX")

using Microsoft::WRL::ComPtr;

namespace WindowsPlatformCommon
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

#define HR(X) if (HRESULT Result = (X); FAILED(Result)) { WindowsPlatformCommon::ReportHResult(Result); }

#endif