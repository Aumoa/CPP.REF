// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT

#include <wrl/client.h>

#define DECLGET(FunctionName, VarName) inline const auto& Get ## FunctionName() const noexcept { return VarName; }

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

#pragma pop_macro("TEXT")

#endif