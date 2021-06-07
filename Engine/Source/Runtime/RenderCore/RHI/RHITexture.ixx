// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHITexture;

import :RHIResource;

/// <summary>
/// Provide functions that control texture resources.
/// </summary>
export class RHITexture : public RHIResource
{
public:
	using Super = RHIResource;

public:
	RHITexture(RHIDevice* device, ID3D12Resource* resource);
	~RHITexture() override;
};