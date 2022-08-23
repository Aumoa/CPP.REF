// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandList.h"

class NullRHIDevice;

class NullRHICommandList : public RHICommandList
{
	friend NullRHIDevice;

private:
	bool _begun = false;

private:
	NullRHICommandList(std::shared_ptr<RHIDevice> device);

public:
	virtual ~NullRHICommandList() noexcept override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void SetComputeRootSignature(RHIRootSignature* rootSignature) override;
	virtual void SetPipelineState(RHIRaytracingPipelineState* pipelineState) override;
	virtual void DispatchRays(const RHIDispatchRaysDesc& dispatchRays) override;
	virtual void ResourceBarrier(std::span<RHIResourceBarrier> barriers) override;
	virtual void SetDescriptorTables(std::span<RHIDescriptorTable*> descriptorTables) override;
	virtual void SetComputeRootDescriptorTable(size_t parameterIndex, uint64 heapPtr) override;
	virtual void CopyResource(RHIResource* dstResource, RHIResource* srcResource) override;
};