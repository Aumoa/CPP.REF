// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGISwapChain.h"
#include "D3D12Texture2D.h"

SDXGISwapChain::SDXGISwapChain(SDXGIFactory* factory, SD3D12Device* device, ComPtr<IDXGISwapChain4> swapChain) : Super(factory, device, nullptr)
	, _swapChain(std::move(swapChain))
{
}

void SDXGISwapChain::Present(int32 vSyncLevel)
{
	HR(_swapChain->Present((UINT)vSyncLevel, 0));
}

void SDXGISwapChain::ResizeBuffers(int32 width, int32 height)
{
	for (size_t i = 0; i < 3; ++i)
	{
		if (_buffers[i])
		{
			DestroyObject(_buffers[i]);
			_buffers[i] = nullptr;
		}
	}

	HR(_swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0));
}

IRHITexture2D* SDXGISwapChain::GetBuffer(int32 index)
{
	SD3D12Texture2D*& buf = _buffers[index];

	if (buf == nullptr)
	{
		ComPtr<ID3D12Resource> resource;
		HR(_swapChain->GetBuffer((UINT)index, IID_PPV_ARGS(&resource)));

#if defined(_DEBUG)
		HR(resource->SetName(std::format(L"SwapChain Buffer[{}]", index).c_str()));
#endif

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

		buf = NewObject<SD3D12Texture2D>(_factory, _device, std::move(resource), ComPtr<ID3D12Resource>(), D3D12_PLACED_SUBRESOURCE_FOOTPRINT(), dest);
	}

	return buf;
}

int32 SDXGISwapChain::GetCurrentBackBufferIndex()
{
	return (int32)_swapChain->GetCurrentBackBufferIndex();
}