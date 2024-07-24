// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:PlatformLocalization;

export import :WindowsPlatformLocalization;

#if PLATFORM_WINDOWS

export using PlatformLocalization = WindowsPlatformLocalization;

#endif