// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "RHIStructures.h"
#include <span>
#include <optional>

namespace libty::inline PlatformGeneric::inline RHI
{
	struct IRHIFactory;
	struct IRHIDevice;

	struct PLATFORMGENERIC_API IRHIFactoryChild : implements(IDisposable)
	{
		GENERATED_BODY(IRHIFactoryChild);

		virtual IRHIFactory* GetFactory() = 0;
	};

	struct PLATFORMGENERIC_API IRHIAdapter : implements(IRHIFactoryChild)
	{
		GENERATED_BODY(IRHIAdapter);

		virtual std::wstring GetDeviceName() = 0;
	};

	struct PLATFORMGENERIC_API IRHIDeviceChild : implements(IRHIFactoryChild)
	{
		GENERATED_BODY(IRHIDeviceChild);

		virtual IRHIDevice* GetDevice() = 0;
	};

	struct PLATFORMGENERIC_API IRHIRootSignature : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIRootSignature);
	};

	struct PLATFORMGENERIC_API IRHIPipelineState : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIPipelineState);
	};

	struct PLATFORMGENERIC_API IRHIResource : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIResource);

		virtual RHIResourceDesc GetDesc() = 0;
	};

	struct PLATFORMGENERIC_API IRHIView : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIView);

		virtual size_t GetViewCount() = 0;
		virtual IRHIResource* GetResource(size_t indexOf) = 0;
	};

	struct PLATFORMGENERIC_API IRHIRenderTargetView : implements(IRHIView)
	{
		GENERATED_BODY(IRHIRenderTargetView);

		virtual void CreateRenderTargetView(size_t index, IRHIResource* pResource, const RHIRenderTargetViewDesc* pDesc) = 0;
	};

	struct PLATFORMGENERIC_API IRHIDepthStencilView : implements(IRHIView)
	{
		GENERATED_BODY(IRHIDepthStencilView);

		virtual void CreateDepthStencilView(size_t index, IRHIResource* pResource, const RHIDepthStencilViewDesc* pDesc) = 0;
	};

	struct PLATFORMGENERIC_API IRHIShaderResourceView : implements(IRHIView)
	{
		GENERATED_BODY(IRHIShaderResourceView);

		virtual void CreateShaderResourceView(size_t index, IRHIResource* pResource, const RHIShaderResourceViewDesc* pDesc) = 0;
		virtual void CreateUnorderedAccessView(size_t index, IRHIResource* pResource, IRHIResource* pCounter, const RHIUnorderedAccessViewDesc* pDesc) = 0;
	};

	struct PLATFORMGENERIC_API IRHICommandAllocator : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHICommandAllocator);

		virtual void Reset() = 0;
	};

	struct PLATFORMGENERIC_API IRHIGraphicsCommandList : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIGraphicsCommandList);

		virtual void Reset(IRHICommandAllocator* pAllocator, IRHIPipelineState* pInitialState) = 0;
		virtual void Close() = 0;
		virtual void ResourceBarrier(std::span<const RHIResourceBarrier> barriers) = 0;
		virtual void ClearRenderTargetView(IRHIRenderTargetView* pRTV, size_t indexOf, const Color& color, std::span<const RectN> rects) = 0;
	};

	struct PLATFORMGENERIC_API IRHIShaderCodeBlob : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIShaderCodeBlob);

		virtual const void* GetBufferPointer() = 0;
		virtual size_t GetBufferSize() = 0;
		virtual ERHIShaderType GetShaderCodeType() = 0;
	};

	struct PLATFORMGENERIC_API IRHIShaderCodeWorkspace : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIShaderCodeWorkspace);

		virtual void AddShaderCode(std::wstring_view name, const RHIShaderCode& code) = 0;
		virtual void Compile() = 0;
		virtual IRHIShaderCodeBlob* GetCompiledShaderCodeBlob(std::string_view entryPointName) = 0;
	};

	struct PLATFORMGENERIC_API IRHIFence : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHIFence);

		virtual Task<> SetEventOnCompletion(uint64 fenceValue, std::optional<std::chrono::milliseconds> timeout) = 0;
		virtual uint64 GetCompletedValue() = 0;
	};

	struct PLATFORMGENERIC_API IRHISwapChain : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHISwapChain);

		virtual void ResizeBuffers(const Vector2N& size) = 0;
		virtual IRHIResource* GetBuffer(size_t index) = 0;
		virtual void Present() = 0;
		virtual size_t GetCurrentBackBufferIndex() = 0;
	};

	struct PLATFORMGENERIC_API IRHICommandQueue : implements(IRHIDeviceChild)
	{
		GENERATED_BODY(IRHICommandQueue);
		
		virtual void ExecuteCommandBuffers(std::span<IRHIGraphicsCommandList* const> commandLists) = 0;
		virtual void Signal(IRHIFence* pFence, uint64 fenceValue) = 0;

		template<class... TArgs>
		void ExecuteCommandBuffers(TArgs&&... commandLists) requires std::constructible_from<const IRHIGraphicsCommandList*, TArgs...>
		{
			ExecuteCommandBuffers(std::vector{ std::forward<TArgs>(commandLists)... });
		}
	};

	struct PLATFORMGENERIC_API IRHIDevice : implements(IRHIFactoryChild)
	{
		GENERATED_BODY(IRHIDevice);

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

	struct PLATFORMGENERIC_API IRHIFactory : implements(IDisposable)
	{
		GENERATED_BODY(IRHIFactory);

		virtual IRHIAdapter* GetAdapter(size_t index) = 0;
		virtual IRHIDevice* CreateDevice(IRHIAdapter* pAdapter) = 0;
		virtual IRHISwapChain* CreateSwapChain(IRHICommandQueue* pQueue, size_t numBuffers) = 0;
	};
}