// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHIResource.h"

struct GAME_API RHIRaytracingRenderTarget
{
	Vector2N Size;
	std::shared_ptr<RHIResource> ColorTarget;

	static RHIRaytracingRenderTarget Create(std::shared_ptr<RHIDevice> device, const Vector2N& textureSize);
};