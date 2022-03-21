// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXDevice.h"
#include "DirectXCommandQueue.h"
#include "DirectXCommandAllocator.h"
#include "DirectXCommandList.h"
#include "DirectXFence.h"
#include "DirectXRenderTargetView.h"
#include "DirectXDepthStencilView.h"
#include "DirectXShaderResourceView.h"
#include "DirectXShaderCodeWorkspace.h"
#include "DirectXRootSignature.h"
#include "DirectXPipelineState.h"
#include "DirectXResource.h"

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

IRHIResource* SDirectXDevice::CreateCommittedResource(const RHIHeapProperties& heapProperties, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* pOptimizedClearValue)
{
	auto& l_heapProperties = reinterpret_cast<const D3D12_HEAP_PROPERTIES&>(heapProperties);
	auto& l_heapFlags = reinterpret_cast<const D3D12_HEAP_FLAGS&>(heapFlags);
	auto& l_desc = reinterpret_cast<const D3D12_RESOURCE_DESC&>(desc);
	auto& l_initialState = reinterpret_cast<const D3D12_RESOURCE_STATES&>(initialState);
	auto* l_pOptimizedClearValue = reinterpret_cast<const D3D12_CLEAR_VALUE*>(pOptimizedClearValue);

	ComPtr<ID3D12Resource> pResource;
	HR(pDevice->CreateCommittedResource(&l_heapProperties, l_heapFlags, &l_desc, l_initialState, l_pOptimizedClearValue, IID_PPV_ARGS(&pResource)));
	return gcnew SDirectXResource(this, std::move(pResource));
}

IRHIRenderTargetView* SDirectXDevice::CreateRenderTargetView(size_t count)
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		.NumDescriptors = (UINT)count
	};

	ComPtr<ID3D12DescriptorHeap> pHeap;
	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pHeap)));

	return gcnew SDirectXRenderTargetView(this, std::move(pHeap), count);
}

IRHIDepthStencilView* SDirectXDevice::CreateDepthStencilView(size_t count)
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		.NumDescriptors = (UINT)count
	};

	ComPtr<ID3D12DescriptorHeap> pHeap;
	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pHeap)));

	return gcnew SDirectXDepthStencilView(this, std::move(pHeap), count);
}

IRHIShaderResourceView* SDirectXDevice::CreateShaderResourceView(size_t count)
{
	D3D12_DESCRIPTOR_HEAP_DESC HeapDesc =
	{
		.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		.NumDescriptors = (UINT)count
	};

	ComPtr<ID3D12DescriptorHeap> pHeap;
	HR(pDevice->CreateDescriptorHeap(&HeapDesc, IID_PPV_ARGS(&pHeap)));

	return gcnew SDirectXShaderResourceView(this, std::move(pHeap), count);
}

IRHIFence* SDirectXDevice::CreateFence()
{
	ComPtr<ID3D12Fence> pFence;
	HR(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)));
	return gcnew SDirectXFence(this, std::move(pFence));
}

IRHICommandAllocator* SDirectXDevice::CreateCommandAllocator(ERHICommandListType type)
{
	ComPtr<ID3D12CommandAllocator> pAllocator;
	HR(pDevice->CreateCommandAllocator(reinterpret_cast<const D3D12_COMMAND_LIST_TYPE&>(type), IID_PPV_ARGS(&pAllocator)));
	return gcnew SDirectXCommandAllocator(this, std::move(pAllocator));
}

IRHIGraphicsCommandList* SDirectXDevice::CreateCommandList(IRHICommandAllocator* pAllocator, ERHICommandListType type, IRHIPipelineState* pInitialState)
{
	auto sAllocator = Cast<SDirectXCommandAllocator>(pAllocator);
	auto sInitialState = Cast<SDirectXPipelineState>(pInitialState);

	ComPtr<ID3D12GraphicsCommandList4> pCommandList;
	HR(pDevice->CreateCommandList(0, reinterpret_cast<const D3D12_COMMAND_LIST_TYPE&>(type), sAllocator->pAllocator.Get(), sInitialState ? sInitialState->pPipelineState.Get() : nullptr, IID_PPV_ARGS(&pCommandList)));
	HR(pCommandList->Close());
	return gcnew SDirectXCommandList(this, sAllocator->pAllocator, std::move(pCommandList));
}

IRHIShaderCodeWorkspace* SDirectXDevice::CreateShaderCodeWorkspace()
{
	return gcnew SDirectXShaderCodeWorkspace(this);
}

IRHIRootSignature* SDirectXDevice::CreateRootSignature(const RHIRootSignatureDesc& RSDesc)
{
	auto& RSDesc_c = reinterpret_cast<const D3D12_ROOT_SIGNATURE_DESC&>(RSDesc);
	
	ComPtr<ID3DBlob> pBlob, pError;
	HRESULT Hr = D3D12SerializeRootSignature(&RSDesc_c, D3D_ROOT_SIGNATURE_VERSION_1_0, &pBlob, &pError);
	if (FAILED(Hr))
	{
		if (pError)
		{
			std::string_view Msg((const char*)pError->GetBufferPointer(), pError->GetBufferSize());
			throw fatal_exception(String::Format("Failed to create root signature: {}", Msg));
		}
		else
		{
			HR(Hr);
		}
	}

	ComPtr<ID3D12RootSignature> pRootSignature;
	HR(pDevice->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&pRootSignature)));

	return gcnew SDirectXRootSignature(this, std::move(pRootSignature));
}

IRHIPipelineState* SDirectXDevice::CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& PSDesc)
{
	auto PSDesc_c = reinterpret_cast<const D3D12_GRAPHICS_PIPELINE_STATE_DESC&>(PSDesc);
	PSDesc_c.pRootSignature = Cast<SDirectXRootSignature>(PSDesc.pRootSignature)->pRootSignature.Get();

	ComPtr<ID3D12PipelineState> pPipelineState;
	HR(pDevice->CreateGraphicsPipelineState(&PSDesc_c, IID_PPV_ARGS(&pPipelineState)));

	return gcnew SDirectXPipelineState(this, std::move(pPipelineState));
}

void SDirectXDevice::Dispose(bool bDisposing)
{
	pDevice.Reset();
	Super::Dispose(bDisposing);
}