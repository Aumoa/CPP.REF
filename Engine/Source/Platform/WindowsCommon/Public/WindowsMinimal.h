// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogWindowsCommon.h"

#undef interface
#define NOMINMAX

#include <Windows.h>
#include <winnt.h>
#include <comdef.h>
#include <Psapi.h>

#pragma pack(push, before_imagehlp, 8)
#include <imagehlp.h>
#pragma pack(pop, before_imagehlp)

#undef GetCommandLine

#include "WindowsErrors.inl"