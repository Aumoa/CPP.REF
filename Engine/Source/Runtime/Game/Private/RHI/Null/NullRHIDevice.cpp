// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Null/NullRHIDevice.h"
#include "RHI/Null/NullRHIFactory.h"
#include "RHI/Null/NullRHICommandQueue.h"
#include "RHI/Null/NullRHISwapChain.h"
#include "RHI/Null/NullRHIFence.h"
#include "RHI/Null/NullRHIShaderBytecode.h"
#include "RHI/Null/NullRHIRootSignature.h"
#include "RHI/Null/NullRHICommandList.h"
#include "RHI/Null/NullRHIResource.h"
#include "RHI/Null/NullRHIRaytracingPipelineState.h"
#include "RHI/Null/NullRHIShaderResourceViewTable.h"
#include "RHI/Null/NullRHIShaderBindingTable.h"
#include "RHI/Null/NullRHIDescriptorTable.h"

NullRHIDevice::NullRHIDevice(std::shared_ptr<RHIFactory> factory)
	: RHIDevice(factory)
{
}

NullRHIDevice::~NullRHIDevice() noexcept
{
}

std::shared_ptr<RHICommandQueue> NullRHIDevice::CreateCommandQueue()
{
	return MakeShared<NullRHICommandQueue>(shared_from_this());
}

std::shared_ptr<RHISwapChain> NullRHIDevice::CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow)
{
	return MakeShared<NullRHISwapChain>(shared_from_this());
}

std::shared_ptr<RHIFence> NullRHIDevice::CreateFence()
{
	return MakeShared<NullRHIFence>(shared_from_this());
}

std::shared_ptr<RHIShaderBytecode> NullRHIDevice::CreateShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength)
{
	return MakeShared<NullRHIShaderBytecode>(shared_from_this(), pShaderBytecode, bytecodeLength);
}

std::shared_ptr<RHIRootSignature> NullRHIDevice::CreateRootSignature(const RHIRootSignatureDesc& desc)
{
	return MakeShared<NullRHIRootSignature>(shared_from_this(), desc);
}

std::shared_ptr<RHICommandList> NullRHIDevice::CreateCommandList()
{
	return MakeShared<NullRHICommandList>(shared_from_this());
}

std::shared_ptr<RHIResource> NullRHIDevice::CreateCommittedResource(const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue)
{
	return MakeShared<NullRHIResource>(shared_from_this());
}

std::shared_ptr<RHIRaytracingPipelineState> NullRHIDevice::CreateRaytracingPipelineState(const RHIShaderLibraryExport& shaderExport)
{
	return MakeShared<NullRHIRaytracingPipelineState>(shared_from_this());
}

std::shared_ptr<RHIShaderResourceViewTable> NullRHIDevice::CreateShaderResourceViewTable(ERHIShaderResourceViewType type, size_t numViews)
{
	return MakeShared<NullRHIShaderResourceViewTable>(shared_from_this(), type, numViews);
}

std::shared_ptr<RHIShaderBindingTable> NullRHIDevice::CreateShaderBindingTable()
{
	return MakeShared<NullRHIShaderBindingTable>(shared_from_this());
}

std::shared_ptr<RHIDescriptorTable> NullRHIDevice::CreateDescriptorTable(size_t descriptors)
{
	return MakeShared<NullRHIDescriptorTable>(shared_from_this(), descriptors);
}