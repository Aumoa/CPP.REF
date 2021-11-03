// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Texture2D.h"
#include "D3D12CommandList.h"
#include "D3D12Device.h"
#include "IApplicationInterface.h"

SD3D12Texture2D::SD3D12Texture2D(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadHeap, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout, const RHITexture2DDesc& desc, bool bAllowInteropResource) : Super(factory, device, resource, uploadHeap)
	, _resource(std::move(resource))
	, _uploadHeap(std::move(uploadHeap))
	, _layout(layout)
	, _desc(desc)
{
	if (_uploadHeap)
	{
		_totalBytes = _uploadHeap->GetDesc().Width;
	}

	if (bAllowInteropResource)
	{
		auto DevInt = _device->Get<ID3D11On12Device>();

		auto Flags = (D3D12_RESOURCE_FLAGS)desc.Flags;
		D3D11_RESOURCE_FLAGS BindFlags =
		{
			.BindFlags = GetInteropBindFlag(Flags)
		};

		D3D12_RESOURCE_STATES InteropInitialState = (Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0 ? D3D12_RESOURCE_STATE_RENDER_TARGET : D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
		HR(DevInt->CreateWrappedResource(_resource.Get(), &BindFlags, InteropInitialState, (D3D12_RESOURCE_STATES)desc.InitialState, IID_PPV_ARGS(&WrappedResource)));

		ComPtr<IDXGISurface> Surf;
		HR(WrappedResource.As(&Surf));

		D2D1_BITMAP_OPTIONS Options = D2D1_BITMAP_OPTIONS_NONE;
		if (Flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET)
		{
			Options = D2D1_BITMAP_OPTIONS_TARGET;
		}

		float dpiScale = IApplicationInterface::Get().GetDpi();
		auto DevCtx2D = _device->Get<ID2D1DeviceContext>();
		HR(DevCtx2D->CreateBitmapFromDxgiSurface(Surf.Get(), D2D1::BitmapProperties1(Options, D2D1::PixelFormat((DXGI_FORMAT)desc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiScale, dpiScale), &Bitmap));
	}
}

void SD3D12Texture2D::UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData)
{
	if (_totalBytes)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = _resource.Get();
		barrier.Transition.StateBefore = (D3D12_RESOURCE_STATES)_desc.InitialState;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.Subresource = 0;

		commandList->ResourceBarrier(1, &barrier);
		UpdateSubresource(commandList, _resource.Get(), _uploadHeap.Get(), subresource, _layout, _totalBytes, uploadData);

		std::swap(barrier.Transition.StateBefore, barrier.Transition.StateAfter);
		commandList->ResourceBarrier(1, &barrier);
	}
	else
	{
		SE_LOG(LogWindows, Error, L"Texture is not created with ERHIBufferUsage::Dynamic flags.");
	}
}

void SD3D12Texture2D::UpdateSubresource(SD3D12CommandList* commandList, ID3D12Resource* textureBuf, ID3D12Resource* uploadBuf, uint32 subresource, const D3D12_PLACED_SUBRESOURCE_FOOTPRINT& layout, uint64 totalBytes, const RHISubresourceData* uploadData)
{
	int8* pData;
	HR(uploadBuf->Map(0, nullptr, reinterpret_cast<void**>(&pData)));
	if (uploadData->SysMemPitch == layout.Footprint.RowPitch)
	{
		memcpy(pData, uploadData->pSysMem, (size_t)totalBytes);
	}
	else
	{
		const size_t pitch = std::min((size_t)layout.Footprint.Width, (size_t)uploadData->SysMemPitch);
		for (uint32 i = 0; i < layout.Footprint.Height; ++i)
		{
			int8*		dst = pData + i * (size_t)layout.Footprint.RowPitch;
			int8 const* src = reinterpret_cast<const int8*>(uploadData->pSysMem) + i * uploadData->SysMemPitch;
			memcpy(dst, src, pitch);
		}
	}
	uploadBuf->Unmap(0, nullptr);

	D3D12_TEXTURE_COPY_LOCATION dst = {};
	dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dst.SubresourceIndex = subresource;
	D3D12_TEXTURE_COPY_LOCATION src = {};
	src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src.PlacedFootprint = layout;

	commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
}