// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHICommandList.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIRootSignature.h"
#include "RHI/Windows/WindowsRHIRaytracingPipelineState.h"
#include "RHI/Windows/WindowsRHIResource.h"
#include "RHI/Windows/WindowsRHIDescriptorTable.h"

std::shared_ptr<RHICommandList> WindowsRHIDevice::CreateCommandList()
{
	return MakeShared<WindowsRHICommandList>(SharedFromThis());
}

WindowsRHICommandList::WindowsRHICommandList(std::shared_ptr<WindowsRHIDevice> device)
	: RHICommandList(device)
{
	auto* d3ddev = WinGetr(device);
	HR(d3ddev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_allocator)));
}

WindowsRHICommandList::~WindowsRHICommandList() noexcept
{
}

void WindowsRHICommandList::BeginFrame()
{
	check(_begun == false);
	
	HR(_allocator->Reset());
	if (_commandList)
	{
		HR(_commandList->Reset(_allocator.Get(), nullptr));
	}
	else
	{
		auto* d3ddev = WinGetr(GetDevice());
		HR(d3ddev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _allocator.Get(), nullptr, IID_PPV_ARGS(&_commandList)));
	}

	_begun = true;
}

void WindowsRHICommandList::EndFrame()
{
	check(_begun);

	HR(_commandList->Close());
	_begun = false;
}

void WindowsRHICommandList::SetComputeRootSignature(RHIRootSignature* rootSignature)
{
	check(_begun);
	_commandList->SetComputeRootSignature(WinGetr(rootSignature));
}

void WindowsRHICommandList::SetPipelineState(RHIRaytracingPipelineState* pipelineState)
{
	check(_begun);
	_commandList->SetPipelineState1(WinGetr(pipelineState));
}

void WindowsRHICommandList::DispatchRays(const RHIDispatchRaysDesc& dispatchRays)
{
	check(_begun);
	_commandList->DispatchRays(reinterpret_cast<const D3D12_DISPATCH_RAYS_DESC*>(&dispatchRays));
}

void WindowsRHICommandList::ResourceBarrier(std::span<RHIResourceBarrier> barriers)
{
	check(_begun);

	static thread_local std::vector<D3D12_RESOURCE_BARRIER> d3dbarriers(barriers.size());
	memcpy(d3dbarriers.data(), barriers.data(), sizeof(D3D12_RESOURCE_BARRIER) * barriers.size());
	for (size_t i = 0; i < barriers.size(); ++i)
	{
		switch (d3dbarriers[i].Type)
		{
		case D3D12_RESOURCE_BARRIER_TYPE_ALIASING:
			d3dbarriers[i].Aliasing.pResourceBefore = WinGetr(barriers[i].Aliasing.pResourceBefore);
			d3dbarriers[i].Aliasing.pResourceAfter = WinGetr(barriers[i].Aliasing.pResourceAfter);
			break;
		case D3D12_RESOURCE_BARRIER_TYPE_TRANSITION:
			d3dbarriers[i].Transition.pResource = WinGetr(barriers[i].Transition.pResource);
			break;
		case D3D12_RESOURCE_BARRIER_TYPE_UAV:
			d3dbarriers[i].UAV.pResource = WinGetr(barriers[i].UAV.pResource);
			break;
		}
	}

	_commandList->ResourceBarrier((UINT)d3dbarriers.size(), d3dbarriers.data());
}

void WindowsRHICommandList::SetDescriptorTables(std::span<RHIDescriptorTable*> descriptorTables)
{
	ID3D12DescriptorHeap* heaps[2] = {};
	size_t index = 0;

	for (size_t i = 0; i < std::min(descriptorTables.size(), SE_ARRAYSIZE(heaps)); ++i)
	{
		heaps[index++] = WinGetr(descriptorTables[i]);
	}

	_commandList->SetDescriptorHeaps((UINT)index, heaps);
}

void WindowsRHICommandList::SetComputeRootDescriptorTable(size_t parameterIndex, uint64 heapPtr)
{
	_commandList->SetComputeRootDescriptorTable((UINT)parameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE{ .ptr = heapPtr });
}

void WindowsRHICommandList::CopyResource(RHIResource* dstResource, RHIResource* srcResource)
{
	_commandList->CopyResource(WinGetr(dstResource), WinGetr(srcResource));
}

#endif