// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHITexture2D;

import :RHITexture;

/// <summary>
/// Represents 2D textures.
/// </summary>
export class RHITexture2D : public RHITexture
{
public:
	using Super = RHITexture;

public:
	RHITexture2D(RHIDevice* device, ID3D12Resource* resource);
	~RHITexture2D() override;
};