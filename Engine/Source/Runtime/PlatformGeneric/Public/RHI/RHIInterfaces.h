// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IDisposable.h"
#include "RHIStructures.h"
#include "Numerics/VectorInterface/Rect.h"
#include "Threading/Task.h"
#include <span>
#include <optional>

struct IRHIFactory;
struct IRHIDevice;

struct IRHIFactoryChild : implements SObject, implements IDisposable
{
	GENERATED_INTERFACE_BODY(IRHIFactoryChild)

	virtual IRHIFactory* GetFactory() = 0;
};

struct IRHIAdapter : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHIAdapter)

	virtual std::wstring GetDeviceName() = 0;
};

struct IRHIDeviceChild : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHIDeviceChild)

	virtual IRHIDevice* GetDevice() = 0;
};

struct IRHIRootSignature : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIRootSignature)
};

struct IRHIPipelineState : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIPipelineState)
};

struct IRHIResource : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIResource)

	virtual RHIResourceDesc GetDesc() = 0;
};

struct IRHIView : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIView)

	virtual size_t GetViewCount() = 0;
	virtual IRHIResource* GetResource(size_t indexOf) = 0;
};

struct IRHIRenderTargetView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIRenderTargetView)

	virtual void CreateRenderTargetView(size_t index, IRHIResource* pResource, const RHIRenderTargetViewDesc* pDesc) = 0;
};

struct IRHIDepthStencilView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIDepthStencilView)

	virtual void CreateDepthStencilView(size_t index, IRHIResource* pResource, const RHIDepthStencilViewDesc* pDesc) = 0;
};

struct IRHIShaderResourceView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIShaderResourceView)

	virtual void CreateShaderResourceView(size_t index, IRHIResource* pResource, const RHIShaderResourceViewDesc* pDesc) = 0;
	virtual void CreateUnorderedAccessView(size_t index, IRHIResource* pResource, IRHIResource* pCounter, const RHIUnorderedAccessViewDesc* pDesc) = 0;
};

struct IRHICommandAllocator : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHICommandAllocator)

	virtual void Reset() = 0;
};

struct IRHIGraphicsCommandList : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIGraphicsCommandList)

	virtual void Reset(IRHICommandAllocator* pAllocator, IRHIPipelineState* pInitialState) = 0;
	virtual void Close() = 0;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> barriers) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* pRTV, size_t indexOf, const Color& color, std::span<const RectN> rects) = 0;
};

struct IRHIShaderCodeBlob : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIShaderCodeBlob)

	virtual const void* GetBufferPointer() = 0;
	virtual size_t GetBufferSize() = 0;
	virtual ERHIShaderType GetShaderCodeType() = 0;
};

struct IRHIShaderCodeWorkspace : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIShaderCodeWorkspace)

	virtual void AddShaderCode(std::wstring_view name, const RHIShaderCode& code) = 0;
	virtual void Compile() = 0;
	virtual IRHIShaderCodeBlob* GetCompiledShaderCodeBlob(std::string_view entryPointName) = 0;
};

struct IRHIFence : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIFence)

	virtual Task<> SetEventOnCompletion(uint64 fenceValue, std::optional<std::chrono::milliseconds> timeout) = 0;
	virtual uint64 GetCompletedValue() = 0;
};

struct IRHISwapChain : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHISwapChain)

	virtual void ResizeBuffers(const Vector2N& size) = 0;
	virtual IRHIResource* GetBuffer(size_t index) = 0;
	virtual void Present() = 0;
	virtual size_t GetCurrentBackBufferIndex() = 0;
};

struct IRHICommandQueue : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHICommandQueue)
		
	virtual void ExecuteCommandBuffers(std::span<IRHIGraphicsCommandList* const> commandLists) = 0;
	virtual void Signal(IRHIFence* pFence, uint64 fenceValue) = 0;

	template<class... TArgs>
	void ExecuteCommandBuffers(TArgs&&... commandLists) requires std::constructible_from<const IRHIGraphicsCommandList*, TArgs...>
	{
		ExecuteCommandBuffers(std::vector{ std::forward<TArgs>(commandLists)... });
	}
};

struct IRHIDevice : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHIDevice)

	virtual IRHICommandQueue* CreateCommandQueue(ERHICommandListType type) = 0;
	virtual IRHIResource* CreateCommittedResource(const RHIHeapProperties& heapProperties, ERHIHeapFlags heapFlags, const RHIResourceDesc& desc, ERHIResourceStates initialState, const RHIClearValue* pOptimizedClearValue) = 0;
	virtual IRHIRenderTargetView* CreateRenderTargetView(size_t count) = 0;
	virtual IRHIDepthStencilView* CreateDepthStencilView(size_t count) = 0;
	virtual IRHIShaderResourceView* CreateShaderResourceView(size_t count) = 0;
	virtual IRHIFence* CreateFence() = 0;
	virtual IRHICommandAllocator* CreateCommandAllocator(ERHICommandListType type) = 0;
	virtual IRHIGraphicsCommandList* CreateCommandList(IRHICommandAllocator* pAllocator, ERHICommandListType type, IRHIPipelineState* pInitialState) = 0;
	virtual IRHIShaderCodeWorkspace* CreateShaderCodeWorkspace() = 0;
	virtual IRHIRootSignature* CreateRootSignature(const RHIRootSignatureDesc& desc) = 0;
	virtual IRHIPipelineState* CreateGraphicsPipelineState(const RHIGraphicsPipelineStateDesc& desc) = 0;
};

struct IRHIFactory : implements IDisposable
{
	GENERATED_INTERFACE_BODY(IRHIFactory)

	virtual IRHIAdapter* GetAdapter(size_t index) = 0;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* pAdapter) = 0;
	virtual IRHISwapChain* CreateSwapChain(IRHICommandQueue* pQueue, size_t numBuffers) = 0;
};