// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Assets/CachedShaderLibrary.h"

#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXRaytracingShader.h"

CachedShaderLibrary::CachedShaderLibrary(DirectXDeviceBundle* deviceBundle) : Super()
	, deviceBundle(deviceBundle)
{

}

CachedShaderLibrary::~CachedShaderLibrary()
{

}

DirectXRaytracingShader* CachedShaderLibrary::GetOrReadyShader()
{
	if (!rtShader.IsValid)
	{
		DirectXNew(rtShader, DirectXRaytracingShader, deviceBundle);
		rtShader->Init();
	}

	return rtShader.Get();
}