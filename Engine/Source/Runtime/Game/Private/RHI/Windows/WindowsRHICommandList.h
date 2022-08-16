// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHICommandList.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIDevice;

class WindowsRHICommandList : public RHICommandList
{
	friend WindowsRHIDevice;

private:
	ComPtr<ID3D12CommandAllocator> _allocator;
	ComPtr<ID3D12GraphicsCommandList4> _commandList;
	bool _begun = false;

private:
	WindowsRHICommandList(std::shared_ptr<WindowsRHIDevice> device);

public:
	virtual ~WindowsRHICommandList() noexcept override;

	virtual void BeginFrame() override;
	virtual void EndFrame() override;

	virtual void SetComputeRootSignature(RHIRootSignature* rootSignature) override;
	virtual void SetPipelineState(RHIRaytracingPipelineState* pipelineState) override;
	virtual void DispatchRays(const RHIDispatchRaysDesc& dispatchRays) override;

public:
	DECLGET(CommandList, _commandList);
};

#endif