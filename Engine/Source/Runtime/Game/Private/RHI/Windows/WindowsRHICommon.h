// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <comdef.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <d3dcompiler.h>

#pragma pop_macro("TEXT")

using Microsoft::WRL::ComPtr;

#include "WindowsRHIMacros.h"
#include "WindowsRHIConvert.h"

#endif