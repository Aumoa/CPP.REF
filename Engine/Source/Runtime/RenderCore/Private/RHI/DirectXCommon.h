// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#pragma push_macro("interface")
#undef interface

#define NOMINMAX

#include <comdef.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#pragma pop_macro("interface")

DECLARE_LOG_CATEGORY(, LogDirectX);

#define HR(x)																				\
if (HRESULT __hr = (x); FAILED(__hr))														\
{																							\
	_com_error com_error(__hr);																\
	SE_LOG(LogDirectX, Fatal, L"An exception occurred with HRESULT: 0x{:08X}, Message: {}",	\
		(uint32)__hr, TCHAR_TO_WCHAR(com_error.ErrorMessage()));								\
}

#define DECLARE_GETTER(Type, Member)								\
	template<std::same_as<Type> T>									\
	inline T* Get() { return static_cast<Type*>(Member.Get()); }

inline UINT GetInteropBindFlag(D3D12_RESOURCE_FLAGS flags)
{
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	return bindFlags;
}