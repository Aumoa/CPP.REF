// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

class DirectXDeviceBundle;
class DirectXRaytracingShader;

class GAME_API CachedShaderLibrary : virtual public Object
{
public:
	using Super = Object;

private:
	DirectXDeviceBundle* deviceBundle;
	TRefPtr<DirectXRaytracingShader> rtShader;

public:
	CachedShaderLibrary(DirectXDeviceBundle* deviceBundle);
	~CachedShaderLibrary() override;

	DirectXRaytracingShader* GetOrReadyShader();
};