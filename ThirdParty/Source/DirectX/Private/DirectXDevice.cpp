// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXDevice.h"
#include "DirectXCommandQueue.h"
#include "DirectXCommandAllocator.h"
#include "DirectXCommandList.h"
#include "DirectXFence.h"
#include "DirectXRenderTargetView.h"
#include "DirectXRenderPass.h"
#include "DirectXShaderCodeWorkspace.h"

GENERATE_BODY(SDirectXDevice);

SDirectXDevice::SDirectXDevice(SDirectXFactory* Owner, ComPtr<ID3D12Device> pDevice)
	: Super(Owner)
	, pDevice(pDevice)
{
}

IRHICommandQueue* SDirectXDevice::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC QueueDesc =
	{
		.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
		.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	};

	ComPtr<ID3D12CommandQueue> pQueue;
	HR(pDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&pQueue)));

	return gcnew SDirectXCommandQueue(this, std::move(pQueue));
}

IRHIRenderTargetView* SDirectXDevice::CreateRenderTargetView(int32 Count)
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		.NumDescriptors = (UINT)Count
	};

	ComPtr<ID3D12DescriptorHeap> pHeap;
	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pHeap)));

	return gcnew SDirectXRenderTargetView(this, std::move(pHeap), (size_t)Count);
}

IRHIFence* SDirectXDevice::CreateFence()
{
	ComPtr<ID3D12Fence> pFence;
	HR(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)));
	return gcnew SDirectXFence(this, std::move(pFence));
}

IRHICommandAllocator* SDirectXDevice::CreateCommandAllocator()
{
	ComPtr<ID3D12CommandAllocator> pAllocator;
	HR(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pAllocator)));
	return gcnew SDirectXCommandAllocator(this, std::move(pAllocator));
}

IRHICommandBuffer* SDirectXDevice::CreateCommandBuffer(IRHICommandAllocator* Allocator)
{
	auto sAllocator = Cast<SDirectXCommandAllocator>(Allocator);
	ComPtr<ID3D12GraphicsCommandList4> pCommandList;
	HR(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, sAllocator->pAllocator.Get(), nullptr, IID_PPV_ARGS(&pCommandList)));
	HR(pCommandList->Close());
	return gcnew SDirectXCommandList(this, sAllocator->pAllocator, std::move(pCommandList));
}

IRHIRenderPass* SDirectXDevice::CreateRenderPass()
{
	return gcnew SDirectXRenderPass(this);
}

IRHIShaderCodeWorkspace* SDirectXDevice::CreateShaderCodeWorkspace()
{
	return gcnew SDirectXShaderCodeWorkspace(this);
}

void SDirectXDevice::Dispose(bool bDisposing)
{
	pDevice.Reset();
	Super::Dispose(bDisposing);
}