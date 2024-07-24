// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:PlatformTypes;

export import :WindowsPlatformTypes;

#if PLATFORM_WINDOWS

using PlatformTypes = WindowsPlatformTypes;

#endif