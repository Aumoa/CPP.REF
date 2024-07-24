// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:WindowsPlatformTypes;

export import :GenericPlatformTypes;

#if PLATFORM_WINDOWS

export struct WindowsPlatformTypes : public GenericPlatformTypes
{
};

#endif