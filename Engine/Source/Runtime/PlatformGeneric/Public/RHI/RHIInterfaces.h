// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "IDisposable.h"
#include "RHIStructures.h"
#include "Numerics/VectorInterface/Rect.h"
#include <span>

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

struct IRHIResource : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIResource)
};

struct IRHIView : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIView)

	virtual int32 GetViewCount() = 0;
	virtual IRHIResource* GetResource(int32 IndexOf) = 0;
};

struct IRHIRenderTargetView : implements IRHIView
{
	GENERATED_INTERFACE_BODY(IRHIRenderTargetView)

	virtual void CreateRenderTargetView(int32 Index, IRHIResource* Resource, const RHIRenderTargetViewDesc* Desc) = 0;
};

struct IRHIRenderPass : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIRenderPass)

	virtual void AddColorAttachment(const RHIRenderPassAttachment& AttachDesc) = 0;
	virtual void Apply(std::span<std::pair<IRHIView*, int32> const> AttachViews, const Vector2N& Size) = 0;
};

struct IRHICommandAllocator : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHICommandAllocator)

	virtual void Reset() = 0;
};

struct IRHICommandBuffer : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHICommandBuffer)

	virtual void BeginRecord() = 0;
	virtual void EndRecord() = 0;
	virtual void ResourceBarrier(std::span<const RHIResourceBarrier> InBarriers) = 0;
	virtual void ClearRenderTargetView(IRHIRenderTargetView* RTV, int32 IndexOf, const Color& InColor) = 0;
	virtual void BeginRenderPass(IRHIRenderPass* RenderPass, const RectN& ScissorRect, std::span<Color const> InColor) = 0;
	virtual void EndRenderPass() = 0;
};

struct IRHITexture2D : implements IRHIResource
{
	GENERATED_INTERFACE_BODY(IRHITexture2D)

	virtual Vector2N GetPixelSize() = 0;
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

	virtual void AddShaderCode(std::wstring_view Name, RHIShaderCode Code) = 0;
	virtual void Compile() = 0;
	virtual IRHIShaderCodeBlob* GetCompiledShaderCodeBlob(std::wstring_view Name) = 0;
};

struct IRHIFence : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHIFence)

	virtual void Wait() = 0;
	virtual bool IsReady() = 0;
};

struct IRHISwapChain : implements IRHIFence
{
	GENERATED_INTERFACE_BODY(IRHISwapChain)

	virtual void ResizeBuffers(const Vector2N& Size) = 0;
	virtual IRHITexture2D* GetBuffer(int32 Index) = 0;
};

struct IRHICommandQueue : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHICommandQueue)
		
	virtual int32 AcquireSwapChainImage(IRHISwapChain* SwapChain) = 0;
	virtual void Present(IRHISwapChain* SwapChain, int32 BufferIndex) = 0;
	virtual void Submit(std::span<IRHICommandBuffer* const> CommandBuffers, IRHIFence* Fence) = 0;

	template<class... TArgs>
	void Submit(IRHIFence* Fence, TArgs&&... CommandBuffers) requires requires { std::initializer_list{ std::declval<TArgs>()... }; }
	{
		Submit(std::initializer_list{ std::forward<TArgs>(CommandBuffers)... }, Fence);
	}
};

struct IRHIDevice : implements IRHIFactoryChild
{
	GENERATED_INTERFACE_BODY(IRHIDevice)

	virtual IRHICommandQueue* CreateCommandQueue() = 0;
	virtual IRHITexture2D* CreateTexture2D(const RHITexture2DDesc& InDesc, const RHISubresourceData* InitialData) = 0;
	virtual IRHIRenderTargetView* CreateRenderTargetView(int32 Count) = 0;
	virtual IRHIFence* CreateFence() = 0;
	virtual IRHICommandAllocator* CreateCommandAllocator() = 0;
	virtual IRHICommandBuffer* CreateCommandBuffer(IRHICommandAllocator* Allocator) = 0;
	virtual IRHIRenderPass* CreateRenderPass() = 0;
	virtual IRHIShaderCodeWorkspace* CreateShaderCodeWorkspace() = 0;
};

struct IRHIFactory : implements IDisposable
{
	GENERATED_INTERFACE_BODY(IRHIFactory)

	virtual IRHIAdapter* GetAdapter(int32 Index) = 0;
	virtual IRHIDevice* CreateDevice(IRHIAdapter* Adapter) = 0;
	virtual IRHISwapChain* CreateSwapChain(IRHICommandQueue* Queue, size_t NumBuffers) = 0;
};