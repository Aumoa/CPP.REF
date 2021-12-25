// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DXGISwapChain.h"
#include "D3D12Texture2D.h"

GENERATE_BODY(SDXGISwapChain);

SDXGISwapChain::SDXGISwapChain(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<IDXGISwapChain4> SwapChain) : Super(InFactory, InDevice, nullptr)
	, SwapChain(std::move(SwapChain))
{
}

void SDXGISwapChain::Present(int32 vSyncLevel)
{
	HR(SwapChain->Present((UINT)vSyncLevel, 0));
}

void SDXGISwapChain::ResizeBuffers(int32 InWidth, int32 InHeight)
{
	for (size_t i = 0; i < 3; ++i)
	{
		if (Buffers[i])
		{
			Buffers[i]->Dispose();
			Buffers[i] = nullptr;
		}
	}

	HR(SwapChain->ResizeBuffers(0, (UINT)InWidth, (UINT)InHeight, DXGI_FORMAT_UNKNOWN, 0));
}

IRHITexture2D* SDXGISwapChain::GetBuffer(int32 Index)
{
	SD3D12Texture2D*& Buf = Buffers[Index];

	if (Buf == nullptr)
	{
		ComPtr<ID3D12Resource> Resource;
		HR(SwapChain->GetBuffer((UINT)Index, IID_PPV_ARGS(&Resource)));

#if DO_CHECK
		HR(Resource->SetName(std::format(L"SwapChain Buffer[{}]", Index).c_str()));
#endif

		D3D12_RESOURCE_DESC source = Resource->GetDesc();
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

		Buf = gcnew SD3D12Texture2D(Factory, Device, std::move(Resource), ComPtr<ID3D12Resource>(), D3D12_PLACED_SUBRESOURCE_FOOTPRINT(), dest);
	}

	return Buf;
}

int32 SDXGISwapChain::GetCurrentBackBufferIndex()
{
	return (int32)SwapChain->GetCurrentBackBufferIndex();
}