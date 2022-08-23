// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDeviceResource.h"

class RHIRootSignature;
class RHIRaytracingPipelineState;
class RHIDescriptorTable;
class RHIResource;
struct RHIDispatchRaysDesc;
struct RHIResourceBarrier;

class GAME_API RHICommandList : public RHIDeviceResource
{
protected:
	RHICommandList(std::shared_ptr<RHIDevice> device);

public:
	virtual ~RHICommandList() noexcept override;

	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void SetComputeRootSignature(RHIRootSignature* rootSignature) = 0;
	virtual void SetPipelineState(RHIRaytracingPipelineState* pipelineState) = 0;
	virtual void DispatchRays(const RHIDispatchRaysDesc& dispatchRays) = 0;
	virtual void ResourceBarrier(std::span<RHIResourceBarrier> barriers) = 0;
	virtual void SetDescriptorTables(std::span<RHIDescriptorTable*> descriptorTables) = 0;
	virtual void SetComputeRootDescriptorTable(size_t parameterIndex, uint64 heapPtr) = 0;
	virtual void CopyResource(RHIResource* dstResource, RHIResource* srcResource) = 0;
};