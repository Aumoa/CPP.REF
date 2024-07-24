// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:PlatformAtomics;

export import :WindowsPlatformAtomics;

#if PLATFORM_WINDOWS

export using PlatformAtomics = WindowsPlatformAtomics;

#endif