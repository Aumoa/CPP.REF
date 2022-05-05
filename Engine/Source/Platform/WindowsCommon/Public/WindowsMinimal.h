// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogWindowsCommon.h"

#undef interface
#define NOMINMAX

#pragma push_macro("TEXT")
#undef TEXT

#include <Windows.h>
#include <winnt.h>
#include <comdef.h>

#pragma pop_macro("TEXT")

#undef GetCommandLine

#include "WindowsErrors.inl"