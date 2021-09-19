// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHITexture.h"

/// <summary>
/// Represents 2D textures.
/// </summary>
class RENDERCORE_API SRHITexture2D : public SRHITexture
{
	GENERATED_BODY(SRHITexture2D)

public:
	SRHITexture2D(SRHIDevice* device, ID3D12Resource* resource);
	~SRHITexture2D() override;

	/// <summary>
	/// Get pixels resolution.
	/// </summary>
	virtual Vector2N GetPixelSize() const;
};