// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_GAME_API__

#pragma warning(disable: 4250)

#include <mutex>

#define GAME_API __declspec(dllexport)

#define ScopedLock(x)\
std::lock_guard<std::mutex> lock_##x(x)

#else
#define GAME_API __declspec(dllimport)
#endif