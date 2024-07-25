// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:PlatformMisc;

export import :WindowsPlatformMisc;

#if PLATFORM_WINDOWS

export using PlatformMisc = WindowsPlatformMisc;

#endif