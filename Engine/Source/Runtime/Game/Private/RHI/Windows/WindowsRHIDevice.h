// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIDevice.h"

#if PLATFORM_WINDOWS

#include "WindowsRHICommon.h"

class WindowsRHIFactory;

class WindowsRHIDevice : public RHIDevice
{
	friend WindowsRHIFactory;

private:
	ComPtr<IDXGIAdapter1> _physicalDevice;
	ComPtr<ID3D12Device5> _device;

private:
	WindowsRHIDevice(std::shared_ptr<WindowsRHIFactory> factory);

public:
	virtual ~WindowsRHIDevice() noexcept override;

	inline ID3D12Device5* GetDevice() const noexcept { return _device.Get(); }

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() override;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) override;
	virtual std::shared_ptr<RHIFence> CreateFence() override;
	virtual std::shared_ptr<RHIShaderBytecode> CreateShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength) override;
	virtual std::shared_ptr<RHIRootSignature> CreateRootSignature(const RHIRootSignatureDesc& desc) override;
	virtual std::shared_ptr<RHICommandList> CreateCommandList() override;
	virtual std::shared_ptr<RHIResource> CreateCommittedResource(const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue) override;
	virtual std::shared_ptr<RHIRaytracingPipelineState> CreateRaytracingPipelineState(const RHIShaderLibraryExport& shaderExport) override;

private:
	inline std::shared_ptr<WindowsRHIDevice> SharedFromThis() noexcept
	{
		return std::static_pointer_cast<WindowsRHIDevice>(shared_from_this());
	}

	template<class T, class... TArgs>
	inline std::shared_ptr<T> MakeShared(TArgs&&... args)
	{
		return std::shared_ptr<T>(new T(std::forward<TArgs>(args)...));
	}
};

#endif