// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#undef interface
#include <dxgi1_6.h>
#include <d3d12.h>
#include "COM/COMMinimal.h"

#define DirectXNew(Ret, Class, ...) \
Ret = Object::NewObject<Class>(__VA_ARGS__);\
Ret->DebugName = L ## #Ret;