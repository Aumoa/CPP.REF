// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#pragma warning(push)
#pragma warning(disable: 4005)

#include <dxgi1_6.h>
#include <d3d12.h>
#include "DirectX/DirectXMinimal.h"

#pragma warning(pop)

#define DirectXNew(Ret, Class, ...) \
Ret = Object::NewObject<Class>(__VA_ARGS__);\
Ret->DebugName = L ## #Ret;