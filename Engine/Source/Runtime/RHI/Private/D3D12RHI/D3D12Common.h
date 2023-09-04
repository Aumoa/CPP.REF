// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#pragma push_macro("NOMINMAX")

#undef TEXT
#undef NOMINMAX

#include "CoreMinimal.h"
#include <dxgi1_6.h>
#include <d3d12.h>
#include <wrl/client.h>

#undef OutputDebugString

#pragma pop_macro("TEXT")
#pragma pop_macro("NOMINMAX")

using Microsoft::WRL::ComPtr;

[[noreturn]]
inline void ReportHResult(HRESULT Result)
{
	checkf(SUCCEEDED(Result), TEXT("Failed to execute D3D12 operation. Error: 0x{:08X}"), Result);
}

#define HR(X) if (HRESULT Result = (X); FAILED(Result)) { ReportHResult(Result); }

#endif