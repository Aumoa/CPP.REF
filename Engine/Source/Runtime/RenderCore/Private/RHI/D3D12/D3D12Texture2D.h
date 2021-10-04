// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Resource.h"
#include "RHI/IRHITexture2D.h"

class SD3D12Texture2D : public SD3D12Resource, implements IRHITexture2D
{
	GENERATED_BODY(SD3D12Texture2D)

private:
	ComPtr<ID3D12Resource> _resource;
	ComPtr<ID3D12Resource> _uploadHeap;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT _layout;

public:
	SD3D12Texture2D(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout);
};