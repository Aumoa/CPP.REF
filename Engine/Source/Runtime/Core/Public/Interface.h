// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#ifdef interface
#undef interface
#endif

#define interface struct __declspec(novtable)