// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGISwapChain.h"
#include "RHI/D3D12/D3D12Texture2D.h"

SDXGISwapChain::SDXGISwapChain(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDXGISwapChain4> swapChain) : Super(factory, device)
	, _swapChain(std::move(swapChain))
{
}

void SDXGISwapChain::Present(int32 vSyncLevel)
{
	HR(_swapChain->Present((UINT)vSyncLevel, 0));
}

void SDXGISwapChain::ResizeBuffers(int32 width, int32 height)
{
	HR(_swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0));
}

IRHITexture2D* SDXGISwapChain::GetBuffer(int32 index)
{
	ComPtr<ID3D12Resource> resource;
	HR(_swapChain->GetBuffer((UINT)index, IID_PPV_ARGS(&resource)));

	D3D12_RESOURCE_DESC source = resource->GetDesc();
	RHITexture2DDesc dest =
	{
		.Width = (uint32)source.Width,
		.Height = (uint32)source.Height,
		.DepthOrArraySize = (uint16)source.DepthOrArraySize,
		.MipLevels = (uint16)source.MipLevels,
		.Format = (ERHIPixelFormat)source.Format,
		.Usage = ERHIBufferUsage::Default,
		.Flags = (ERHIResourceFlags)source.Flags,
		.InitialState = ERHIResourceStates::Present
	};

	return NewObject<SD3D12Texture2D>(_factory, _device, std::move(resource), ComPtr<ID3D12Resource>(), D3D12_PLACED_SUBRESOURCE_FOOTPRINT(), dest);
}

int32 SDXGISwapChain::GetCurrentBackBufferIndex()
{
	return (int32)_swapChain->GetCurrentBackBufferIndex();
}