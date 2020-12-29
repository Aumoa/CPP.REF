// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#ifdef __TH_API__
#define TH_API __declspec(dllexport)

#include "Common/THLogs.h"
#else
#define TH_API __declspec(dllimport)
#endif