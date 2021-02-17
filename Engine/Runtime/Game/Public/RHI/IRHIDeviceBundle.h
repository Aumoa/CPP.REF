// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

#include <span>
#include "RHITextureFormat.h"

interface IRHISwapChain;
interface IRHIImmediateCommandList;
interface IRHICommandFence;
interface IRHIRenderTargetView;
interface IRHIResource;
interface IRHIDeferredCommandList;
interface IRHIFence;
interface IRHIShader;
interface IRHIDepthStencilView;
interface IRHIShaderResourceView;
interface IRHIRenderTarget;
interface IRHIOnlineDescriptorPatch;
enum class ERHIResourceFlags;
enum class ERHIResourceStates;
class RHIShaderLibrary;
class RHIResourceGC;
class PlatformImage;
struct RHIVertex;
struct RHITextureClearValue;

interface GAME_API IRHIDeviceBundle : virtual public Object, virtual public IRHIBundle
{
	virtual IRHISwapChain* GetSwapChain() const = 0;
	virtual IRHIImmediateCommandList* GetImmediateCommandList() const = 0;
	virtual RHIShaderLibrary* GetShaderLibrary() const = 0;
	virtual RHIResourceGC* GetResourceGC() const = 0;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence() = 0;
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource) = 0;
	virtual TRefPtr<IRHIDepthStencilView> CreateDepthStencilView(IRHIResource* resource, ERHITextureFormat inViewFormat = ERHITextureFormat::Unknown) = 0;
	virtual TRefPtr<IRHIShaderResourceView> CreateTextureView(IRHIResource* resource, ERHITextureFormat inViewFormat = ERHITextureFormat::Unknown) = 0;
	virtual TRefPtr<IRHIShaderResourceView> CreateTextureGroupView(std::span<IRHIResource*> inResources) = 0;
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList() = 0;
	virtual TRefPtr<IRHIFence> CreateFence() = 0;
	virtual TRefPtr<IRHIOnlineDescriptorPatch> CreateOnlineDescriptorPatch() = 0;

	virtual TRefPtr<IRHIRenderTarget> CreateGBufferRenderTarget() = 0;
	virtual TRefPtr<IRHIRenderTarget> CreateHDRRenderTarget() = 0;
	virtual TRefPtr<IRHIResource> CreateVertexBuffer(std::span<RHIVertex> vertices) = 0;
	virtual TRefPtr<IRHIResource> CreateIndexBuffer(std::span<uint32> indices) = 0;
	virtual TRefPtr<IRHIResource> CreateDynamicBuffer(size_t sizeInBytes) = 0;
	virtual TRefPtr<IRHIResource> CreateImmutableBuffer(size_t sizeInBytes, ERHIResourceStates initialState) = 0;
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, int32 width, int32 height, ERHIResourceStates initialStates, ERHIResourceFlags flags, const RHITextureClearValue& inClearValue) = 0;
	virtual TRefPtr<IRHIResource> CreateTexture2D(ERHITextureFormat format, PlatformImage* platformImage) = 0;

	virtual void UpdateTextureGroupView(IRHIShaderResourceView* inView, std::span<IRHIResource*> inResources) = 0;
};