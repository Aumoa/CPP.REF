// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#pragma pack(push, 8)

#include <Windows.h>
#include <minwindef.h>
#include <rpc.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Psapi.h>
#include <imagehlp.h>
#include <signal.h>

#undef YieldProcessor
#undef OutputDebugString
#undef GetMessage
#undef GetCurrentDirectory
#undef SetCurrentDirectory
#undef Yield

#pragma pack(pop)

#pragma pop_macro("TEXT")

#endif