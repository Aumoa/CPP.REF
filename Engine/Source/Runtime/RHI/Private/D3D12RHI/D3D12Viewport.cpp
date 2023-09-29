// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Viewport.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Texture2D.h"

ND3D12Viewport::ND3D12Viewport(IDXGIFactory7* InFactory, ID3D12CommandQueue* InQueue, HWND hWnd)
{
	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);

	DXGI_SWAP_CHAIN_DESC1 Desc =
	{
		.Width = (UINT)(ClientRect.right - ClientRect.left),
		.Height = (UINT)(ClientRect.bottom - ClientRect.top),
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = FALSE,
		.SampleDesc = { .Count = 1, .Quality = 0 },
		.BufferUsage = DXGI_USAGE_BACK_BUFFER,
		.BufferCount = (UINT)NumBuffers,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0
	};

	ComPtr<IDXGISwapChain1> SwapChain1;
	HR(InFactory->CreateSwapChainForHwnd(InQueue, hWnd, &Desc, nullptr, nullptr, &SwapChain1));
	HR(SwapChain1.As(&SwapChain4));

	HR(InQueue->GetDevice(IID_PPV_ARGS(&pDevice)));

	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		.NumDescriptors = (UINT)NumBuffers,
		.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE
	};

	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&RTVHeap)));
	RTVIncrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	AllocateBuffersAndCreateDescriptors();

	VpSize = { (int32)Desc.Width, (int32)Desc.Height };
}

ND3D12Viewport::~ND3D12Viewport() noexcept
{
}

void ND3D12Viewport::Present()
{
	HR(SwapChain4->Present(1, 0));
}

Vector2N ND3D12Viewport::GetViewportSize() const
{
	return VpSize;
}

void ND3D12Viewport::Resize(const Vector2N& InSize)
{
	if (InSize.X > 0 && InSize.Y > 0)
	{
		Buffers.clear();
		HR(SwapChain4->ResizeBuffers((UINT)NumBuffers, (UINT)InSize.X, (UINT)InSize.Y, DXGI_FORMAT_UNKNOWN, 0));
		AllocateBuffersAndCreateDescriptors();
	}
	VpSize = InSize;
}

D3D12_CPU_DESCRIPTOR_HANDLE ND3D12Viewport::GetRTVHandle(int32 InIndex) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE hRTV = RTVHeap->GetCPUDescriptorHandleForHeapStart();
	hRTV.ptr += (SIZE_T)RTVIncrementSize * InIndex;
	return hRTV;
}

void ND3D12Viewport::AllocateBuffersAndCreateDescriptors()
{
	Buffers.resize(NumBuffers);
	D3D12_RENDER_TARGET_VIEW_DESC RTVDesc =
	{
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D
	};
	D3D12_CPU_DESCRIPTOR_HANDLE hRTV = RTVHeap->GetCPUDescriptorHandleForHeapStart();

	for (size_t i = 0; i < NumBuffers; ++i)
	{
		ComPtr<ID3D12Resource> pBuffer;
		HR(SwapChain4->GetBuffer((UINT)i, IID_PPV_ARGS(&pBuffer)));
		auto Desc = pBuffer->GetDesc();
		Buffers[i] = std::make_shared<ND3D12Texture2D>(pBuffer, Desc);
		pDevice->CreateRenderTargetView(pBuffer.Get(), &RTVDesc, hRTV);

		// increment handle for next buffer.
		hRTV.ptr += (SIZE_T)RTVIncrementSize;
	}
}

#endif