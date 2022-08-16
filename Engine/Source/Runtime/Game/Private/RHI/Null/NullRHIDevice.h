// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDevice.h"

class NullRHIFactory;

class NullRHIDevice : public RHIDevice
{
	friend NullRHIFactory;

private:
	NullRHIDevice(std::shared_ptr<RHIFactory> factory);

public:
	virtual ~NullRHIDevice() noexcept override;

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) override;
	virtual std::shared_ptr<RHIFence> CreateFence() override;
	virtual std::shared_ptr<RHIShaderBytecode> CreateShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength) override;
	virtual std::shared_ptr<RHIRootSignature> CreateRootSignature(const RHIRootSignatureDesc& desc) override;
	virtual std::shared_ptr<RHICommandList> CreateCommandList() override;
	virtual std::shared_ptr<RHIResource> CreateCommittedResource(const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue) override;
	virtual std::shared_ptr<RHIRaytracingPipelineState> CreateRaytracingPipelineState(const RHIShaderLibraryExport& shaderExport) override;
	virtual std::shared_ptr<RHIShaderResourceViewTable> CreateShaderResourceViewTable(ERHIShaderResourceViewType type, size_t numViews) override;

private:
	template<class T, class... TArgs>
	inline std::shared_ptr<T> MakeShared(TArgs&&... args)
	{
		return std::shared_ptr<T>(new T(std::forward<TArgs>(args)...));
	}
};