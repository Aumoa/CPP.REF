// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12SwapChain.h"

#include "Logging/LogMacros.h"
#include "D3D12Resource.h"

D3D12SwapChain::D3D12SwapChain(IDXGISwapChain4* swapChain) : Super()
	, bufferCount(0)
{
	this->swapChain = swapChain;
	DXGI_SWAP_CHAIN_DESC1 desc;
	HR(swapChain->GetDesc1(&desc));
	bufferCount = (size_t)desc.BufferCount;
	backBuffers.resize(bufferCount);
	AllocBuffers();
}

D3D12SwapChain::~D3D12SwapChain()
{
	ReleaseBuffers();
}

void D3D12SwapChain::Present()
{
	HR(swapChain->Present(0, 0));
}

void D3D12SwapChain::ResizeBuffers(int32 width, int32 height)
{
	ReleaseBuffers();
	HR(swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0));
	AllocBuffers();
}

IRHIResource* D3D12SwapChain::GetBuffer(size_t index) const
{
	if (index >= backBuffers.size())
	{
		SE_LOG(LogD3D12RHI, Error, L"Index out of range.(index: {0}, count: {1})", index, bufferCount);
		return nullptr;
	}

	return backBuffers[index].Get();
}

size_t D3D12SwapChain::CurrentBackBufferIndex_get() const
{
	return (size_t)swapChain->GetCurrentBackBufferIndex();
}

size_t D3D12SwapChain::NumBuffers_get() const
{
	return bufferCount;
}

void D3D12SwapChain::ReleaseBuffers()
{
	for (auto& item : backBuffers)
	{
		item.Reset();
	}
}

void D3D12SwapChain::AllocBuffers()
{
	ComPtr<ID3D12Resource> resource;
	for (size_t i = 0; i < backBuffers.size(); ++i)
	{
		HR(swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&resource)));
		backBuffers[i] = NewObject<D3D12Resource>(resource.Get());
	}
}