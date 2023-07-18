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

#define SIGINT          2   // interrupt
#define SIGILL          4   // illegal instruction - invalid function image
#define SIGFPE          8   // floating point exception
#define SIGSEGV         11  // segment violation
#define SIGTERM         15  // Software termination signal from kill
#define SIGBREAK        21  // Ctrl-Break sequence
#define SIGABRT         22  // abnormal termination triggered by abort call

#pragma pack(pop)

#pragma pop_macro("TEXT")

#endif