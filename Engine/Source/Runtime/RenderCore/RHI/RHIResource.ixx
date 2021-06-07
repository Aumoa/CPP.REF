// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHIResource;

import :RHIDeviceChild;
import :ComPtr;

/// <summary>
/// Represents render resources.
/// </summary>
export class RHIResource : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	ComPtr<ID3D12Resource> _resource;

public:
	RHIResource(RHIDevice* device, ID3D12Resource* resource);
	~RHIResource() override;

public /*internal*/:
	ID3D12Resource* GetResource() const { return _resource.Get(); }
};