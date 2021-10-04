// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Texture2D.h"

SD3D12Texture2D::SD3D12Texture2D(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout) : Super(factory, device, resource)
	, _resource(std::move(resource))
	, _uploadHeap(std::move(uploadHeap))
	, _layout(layout)
{
}