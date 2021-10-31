// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "LogWindows.h"

#undef interface
#define NOMINMAX

#include <comdef.h>
#include <Windows.h>
#include <wrl/client.h>
#include <comdef.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>

#undef GetCommandLine
using Microsoft::WRL::ComPtr;

#include "WindowsErrors.inl"