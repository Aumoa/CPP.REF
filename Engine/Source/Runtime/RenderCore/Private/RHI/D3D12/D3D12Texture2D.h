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
	RHITexture2DDesc _desc;
	uint64 _totalBytes = 0;

	ComPtr<ID3D11Resource> _wrappedResource;
	ComPtr<ID2D1Bitmap1> _bitmap;

public:
	SD3D12Texture2D(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout, const RHITexture2DDesc& desc);

	virtual RHITexture2DDesc GetDesc() override { return _desc; }

	virtual void UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData) override;

	static void UpdateSubresource(
		SD3D12CommandList* commandList,
		ID3D12Resource* textureBuf,
		ID3D12Resource* uploadBuf,
		uint32 subresource,
		const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout,
		uint64 totalBytes,
		const RHISubresourceData* uploadData);
};