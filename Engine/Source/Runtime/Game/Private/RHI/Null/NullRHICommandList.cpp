// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHICommandList.h"

NullRHICommandList::NullRHICommandList(std::shared_ptr<RHIDevice> device)
	: RHICommandList(std::move(device))
{
}

NullRHICommandList::~NullRHICommandList() noexcept
{
}

void NullRHICommandList::BeginFrame()
{
	check(_begun == false);
	_begun = true;
}

void NullRHICommandList::EndFrame()
{
	check(_begun);
	_begun = false;
}

void NullRHICommandList::SetComputeRootSignature(RHIRootSignature* rootSignature)
{
	check(_begun);
	check(rootSignature);
}

void NullRHICommandList::SetPipelineState(RHIRaytracingPipelineState* pipelineState)
{
	check(_begun);
	check(pipelineState);
}

void NullRHICommandList::DispatchRays(const RHIDispatchRaysDesc& dispatchRays)
{
	check(_begun);
}

void NullRHICommandList::ResourceBarrier(std::span<RHIResourceBarrier> barriers)
{
	check(_begun);
}

void NullRHICommandList::SetDescriptorTables(std::span<RHIDescriptorTable*> descriptorTables)
{
	check(_begun);
}

void NullRHICommandList::SetComputeRootDescriptorTable(size_t parameterIndex, uint64 heapPtr)
{
	check(_begun);
}

void NullRHICommandList::CopyResource(RHIResource* dstResource, RHIResource* srcResource)
{
	check(_begun);
}