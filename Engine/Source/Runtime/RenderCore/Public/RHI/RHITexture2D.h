// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHITexture.h"

/// <summary>
/// Represents 2D textures.
/// </summary>
class RENDERCORE_API RHITexture2D : public RHITexture
{
public:
	using Super = RHITexture;

public:
	RHITexture2D(RHIDevice* device, ID3D12Resource* resource);
	~RHITexture2D() override;

	/// <summary>
	/// Get pixels resolution.
	/// </summary>
	virtual void GetPixelSize(int32* pWidth, int32* pHeight);
};