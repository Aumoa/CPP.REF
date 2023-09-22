// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

[[noreturn]]
inline void ReportHResult(HRESULT Result)
{
	checkf(SUCCEEDED(Result), TEXT("Failed to execute D3D12 operation. Error: 0x{:08X}"), Result);
}

#include "D3D12ResourceHelpers.h"

#undef __ALLOW_PLATFORM_COMMON_H__

#endif