// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_GAME_API__
#define GAME_API __declspec(dllexport)
#else
#define GAME_API __declspec(dllimport)
#endif