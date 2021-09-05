// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIResource.h"

/// <summary>
/// Provide functions that control texture resources.
/// </summary>
class RENDERCORE_API RHITexture : public RHIResource
{
	GENERATED_BODY(RHITexture)

public:
	RHITexture(RHIDevice* device, ID3D12Resource* resource);
	~RHITexture() override;
};