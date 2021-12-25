// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Texture2D.h"
#include "D3D12CommandList.h"
#include "D3D12Device.h"
#include "IApplicationInterface.h"

GENERATE_BODY(SD3D12Texture2D);

SD3D12Texture2D::SD3D12Texture2D(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> Resource, ComPtr<ID3D12Resource> UploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& Layout, const RHITexture2DDesc& Desc)
	: Super(InFactory, InDevice, Resource.Get(), UploadHeap.Get())
	, Resource(std::move(Resource))
	, UploadHeap(std::move(UploadHeap))
	, Layout(Layout)
	, Desc(Desc)
{
	if (UploadHeap)
	{
		TotalBytes = UploadHeap->GetDesc().Width;
	}
}

RHITexture2DDesc SD3D12Texture2D::GetDesc()
{
	return Desc;
}

void SD3D12Texture2D::UpdateSubresource(SD3D12CommandList* CommandList, uint32 Subresource, const RHISubresourceData* UploadData)
{
	if (TotalBytes)
	{
		D3D12_RESOURCE_BARRIER Barrier = {};
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Transition.pResource = Resource.Get();
		Barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)Desc.InitialState;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		Barrier.Transition.Subresource = 0;

		CommandList->ResourceBarrier(1, &Barrier);
		UpdateSubresource(CommandList, Resource.Get(), UploadHeap.Get(), Subresource, Layout, TotalBytes, UploadData);

		std::swap(Barrier.Transition.StateBefore, Barrier.Transition.StateAfter);
		CommandList->ResourceBarrier(1, &Barrier);
	}
	else
	{
		SE_LOG(LogDirectX, Error, L"Texture is not created with ERHIBufferUsage::Dynamic flags.");
	}
}

void SD3D12Texture2D::UpdateSubresource(SD3D12CommandList* CommandList, ID3D12Resource* TextureBuf, ID3D12Resource* UploadBuf, uint32 Subresource, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& Layout, uint64 TotalBytes, const RHISubresourceData* UploadData)
{
	int8* pData;
	HR(UploadBuf->Map(0, nullptr, reinterpret_cast<void**>(&pData)));
	if (UploadData->SysMemPitch == Layout.Footprint.RowPitch)
	{
		memcpy(pData, UploadData->pSysMem, (size_t)TotalBytes);
	}
	else
	{
		const size_t pitch = (size_t)UploadData->SysMemPitch;
		for (uint32 i = 0; i < Layout.Footprint.Height; ++i)
		{
			int8* Dst = pData + i * (size_t)Layout.Footprint.RowPitch;
			int8 const* Src = reinterpret_cast<const int8*>(UploadData->pSysMem) + i * UploadData->SysMemPitch;
			memcpy(Dst, Src, pitch);
		}
	}
	UploadBuf->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION Dst = {};
	Dst.pResource = TextureBuf;
	Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dst.SubresourceIndex = Subresource;
	D3D12_TEXTURE_COPY_LOCATION Src = {};
	Src.pResource = UploadBuf;
	Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = Layout;

	CommandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
}

void SD3D12Texture2D::Dispose(bool bDisposing)
{
	Resource.Reset();
	UploadHeap.Reset();

	Super::Dispose(bDisposing);
}