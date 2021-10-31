// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "WindowsIncludes.h"

#define HR(x)								\
if (HRESULT __HR__ = (x); FAILED(__HR__))	\
{											\
	ReportCOMError(L ## #x, __HR__);		\
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