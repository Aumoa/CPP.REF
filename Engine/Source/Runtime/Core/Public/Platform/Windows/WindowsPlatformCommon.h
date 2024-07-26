// Copyright 2020-2024 Aumoa.lib. All right reserved.

#pragma once

#include "Platform/Generic/GenericPlatformCommon.h"

#if PLATFORM_WINDOWS

#include "System/AssertionMacros.h"

#pragma push_macro("TEXT")

#undef TEXT

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

#define HR(X) if (HRESULT Result = (X); FAILED(Result)) { WindowsPlatformCommon::ReportHResult(Result); }

#endif