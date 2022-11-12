// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#include "CoreMinimal.h"

#undef interface
#define NOMINMAX

#pragma push_macro("TEXT")
#undef TEXT

#pragma pack(push, 8)

#include <Windows.h>
#include <winnt.h>
#include <comdef.h>
#include <Psapi.h>
#include <imagehlp.h>

#pragma pack(pop)

#pragma pop_macro("TEXT")

#undef GetCommandLine
#undef Yield
#undef GetMessage
#undef GetObject

constexpr LogCategory LogWindows(TEXT("LogWindow"));

#endif