// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/EnableSharedFromThis.h"

class RHIFactory;
class RHICommandQueue;
class RHISwapChain;
class RHIFence;
class RHIShaderBytecode;
class RHIRootSignature;
class RHICommandList;
class RHIResource;
class RHIRaytracingPipelineState;
interface IPlatformWindow;
struct RHIRootSignatureDesc;
struct RHIResourceDesc;
struct RHIClearValue;
struct RHIHeapProperties;
struct RHIShaderLibraryExport;
enum class ERHIHeapFlags;
enum class ERHIResourceStates;

class GAME_API RHIDevice : public EnableSharedFromThis<RHIDevice>
{
	std::shared_ptr<RHIFactory> _factory;

protected:
	RHIDevice(std::shared_ptr<RHIFactory> factory);

public:
	virtual ~RHIDevice() noexcept;

	std::shared_ptr<RHIFactory> GetFactory() const;

	virtual std::shared_ptr<RHICommandQueue> CreateCommandQueue() = 0;
	virtual std::shared_ptr<RHISwapChain> CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow) = 0;
	virtual std::shared_ptr<RHIFence> CreateFence() = 0;
	virtual std::shared_ptr<RHIShaderBytecode> CreateShaderBytecode(const void* pShaderBytecode, size_t bytecodeLength) = 0;
	virtual std::shared_ptr<RHIRootSignature> CreateRootSignature(const RHIRootSignatureDesc& desc) = 0;
	virtual std::shared_ptr<RHICommandList> CreateCommandList() = 0;
	virtual std::shared_ptr<RHIResource> CreateCommittedResource(const RHIHeapProperties& heapProps, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* clearValue) = 0;
	virtual std::shared_ptr<RHIRaytracingPipelineState> CreateRaytracingPipelineState(const RHIShaderLibraryExport& shaderExport) = 0;
};