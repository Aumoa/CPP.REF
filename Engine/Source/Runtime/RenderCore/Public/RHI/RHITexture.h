// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIResource.h"

/// <summary>
/// Provide functions that control texture resources.
/// </summary>
class RENDERCORE_API SRHITexture : public SRHIResource
{
	GENERATED_BODY(SRHITexture)

public:
	SRHITexture(SRHIDevice* device, ID3D12Resource* resource);
	~SRHITexture() override;
};