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
};