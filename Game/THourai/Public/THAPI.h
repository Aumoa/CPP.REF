// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __TH_API__
#define TH_API __declspec(dllexport)
#else
#define TH_API __declspec(dllimport)
#endif