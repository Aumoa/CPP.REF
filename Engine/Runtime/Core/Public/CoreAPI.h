// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __SC_RUNTIME_CORE_API__
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif