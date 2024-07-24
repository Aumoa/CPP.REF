// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:PlatformProcess;

export import :WindowsPlatformProcess;

#if PLATFORM_WINDOWS

export using PlatformProcess = WindowsPlatformProcess;

#endif