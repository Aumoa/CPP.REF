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

#if DO_CHECK

#define HR(x) \
{ \
	HRESULT hr = (x); \
	auto p = (const TCHAR*)_com_error(hr).Description(); \
	checkf(SUCCEEDED(hr) && #x, TEXT("DirectX RHI assertion failure. HRESULT: {0:X}, Message: {1}"), (int32)hr, p ? String(p) : TEXT("No description.")); \
}

#else

#define HR(x) (x)

#endif

#include "WindowsRHIConvert.h"

#endif