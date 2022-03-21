// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXSwapChain.h"
#include "DirectXFence.h"
#include "DirectXDevice.h"
#include "DirectXResource.h"

GENERATE_BODY(SDirectXSwapChain);

SDirectXSwapChain::SDirectXSwapChain(SDirectXDevice* Owner, ComPtr<IDXGISwapChain4> pSwapChain, size_t BufferCount)
	: Super(Owner)
	, pSwapChain(std::move(pSwapChain))
{
	Buffers.resize(BufferCount);
}

void SDirectXSwapChain::ResizeBuffers(const Vector2N& Size)
{
	for (size_t i = 0; i < Buffers.size(); ++i)
	{
		if (Buffers[i])
		{
			Buffers[i]->Dispose();
			Buffers[i] = nullptr;
		}
	}

	HR(pSwapChain->ResizeBuffers(0, (UINT)Size.X, (UINT)Size.Y, DXGI_FORMAT_UNKNOWN, 0));

	for (size_t i = 0; i < Buffers.size(); ++i)
	{
		ComPtr<ID3D12Resource> pResource;
		HR(this->pSwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&pResource)));
		Buffers[i] = gcnew SDirectXResource(Cast<SDirectXDevice>(GetDevice()), std::move(pResource));
	}
}

IRHIResource* SDirectXSwapChain::GetBuffer(size_t index)
{
	return Buffers[index];
}

void SDirectXSwapChain::Present()
{
	HR(pSwapChain->Present(0, 0));
}

size_t SDirectXSwapChain::GetCurrentBackBufferIndex()
{
	return (size_t)pSwapChain->GetCurrentBackBufferIndex();
}

void SDirectXSwapChain::Dispose(bool bDisposing)
{
	pSwapChain.Reset();
	Buffers.clear();
	Super::Dispose(bDisposing);
}