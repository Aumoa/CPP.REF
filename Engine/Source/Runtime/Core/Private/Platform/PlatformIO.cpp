// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:PlatformIO;

export import :WindowsPlatformIO;

#if PLATFORM_WINDOWS

export using PlatformIO = WindowsPlatformIO;

#endif