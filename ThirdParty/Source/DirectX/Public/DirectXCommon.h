// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowsMinimal.h"
#include <wrl/client.h>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <dxgi1_6.h>
#include <d2d1_1.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>

DECLARE_LOG_CATEGORY(DIRECTX_API, LogDirectX);

#define DECLARE_GETTER(Type, Member)												\
	template<std::same_as<Type> T> requires requires { Member.Get(); }				\
	inline T* Get(short) { return static_cast<Type*>(Member.Get()); }				\
	template<std::same_as<Type> T> requires requires { static_cast<Type*>(Member); }\
	inline T* Get(int) { return static_cast<Type*>(Member); }						\
	template<std::same_as<Type> T>													\
	inline T* Get() { return Get<T>(0); }

inline UINT GetInteropBindFlag(D3D12_RESOURCE_FLAGS flags)
{
	UINT bindFlags = D3D11_BIND_SHADER_RESOURCE;
	if (flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
	{
		bindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	return bindFlags;
}

using Microsoft::WRL::ComPtr;