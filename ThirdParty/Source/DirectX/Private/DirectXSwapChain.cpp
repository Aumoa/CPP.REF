// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXSwapChain.h"
#include "DirectXFence.h"
#include "DirectXDevice.h"
#include "DirectXTexture2D.h"

GENERATE_BODY(SDirectXSwapChain);

SDirectXSwapChain::SDirectXSwapChain(SDirectXDevice* Owner, ComPtr<IDXGISwapChain4> pSwapChain, size_t BufferCount)
	: Super(Owner)
	, pSwapChain(std::move(pSwapChain))
{
	Buffers.resize(BufferCount);
	Owner->pDevice->CreateFence(FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence));
	hFenceEvent = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
}

bool SDirectXSwapChain::IsReady()
{
	return pFence->GetCompletedValue() >= FenceValue;
}

void SDirectXSwapChain::Wait()
{
	if (IsReady())
	{
		HR(pFence->SetEventOnCompletion(FenceValue, hFenceEvent));
		WaitForSingleObject(hFenceEvent, INFINITE);
	}
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
		Buffers[i] = gcnew SDirectXTexture2D(Cast<SDirectXDevice>(GetDevice()), std::move(pResource));
	}
}

IRHITexture2D* SDirectXSwapChain::GetBuffer(int32 Index)
{
	return Buffers[Index];
}

void SDirectXSwapChain::Dispose(bool bDisposing)
{
	if (hFenceEvent)
	{
		CloseHandle(hFenceEvent);
		hFenceEvent = nullptr;
	}

	pSwapChain.Reset();
	Super::Dispose(bDisposing);
}