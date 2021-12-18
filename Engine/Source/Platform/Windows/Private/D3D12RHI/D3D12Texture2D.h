// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Resource.h"
#include "RHI/IRHITexture2D.h"

class SD3D12Texture2D : public SD3D12Resource, implements IRHITexture2D
{
	GENERATED_BODY(SD3D12Texture2D)

private:
	ComPtr<ID3D12Resource> Resource;
	ComPtr<ID3D12Resource> UploadHeap;
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT Layout;
	RHITexture2DDesc Desc;
	uint64 TotalBytes = 0;

public:
	SD3D12Texture2D(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> Resource, ComPtr<ID3D12Resource> UploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& Layout, const RHITexture2DDesc& Desc);

	using Super::Dispose;
	virtual RHITexture2DDesc GetDesc() override;
	virtual void UpdateSubresource(SD3D12CommandList* CommandList, uint32 Subresource, const RHISubresourceData* UploadData) override;

	static void UpdateSubresource(
		SD3D12CommandList* CommandList,
		ID3D12Resource* TextureBuf,
		ID3D12Resource* UploadBuf,
		uint32 Subresource,
		const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& Layout,
		uint64 TotalBytes,
		const RHISubresourceData* UploadData);

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12Resource, Resource);
};