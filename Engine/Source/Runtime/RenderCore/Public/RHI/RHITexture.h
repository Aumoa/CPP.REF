// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIResource.h"

/// <summary>
/// Provide functions that control texture resources.
/// </summary>
class RENDERCORE_API RHITexture : public RHIResource
{
public:
	using Super = RHIResource;

public:
	RHITexture(RHIDevice* device, ID3D12Resource* resource);
	~RHITexture() override;
};