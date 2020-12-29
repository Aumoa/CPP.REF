// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "IRHIBundle.h"

interface IRHISwapChain;
interface IRHIImmediateCommandList;
interface IRHICommandFence;
interface IRHIRenderTargetView;
interface IRHIResource;
interface IRHIDeferredCommandList;

enum class RHITextureFormat;
enum class RHIResourceFlags;
enum class RHIResourceStates;

interface IRHIDeviceBundle : virtual public Object, virtual public IRHIBundle
{
	virtual TRefPtr<IRHISwapChain> GetSwapChain() const = 0;
	virtual TRefPtr<IRHIImmediateCommandList> GetImmediateCommandList() const = 0;

	virtual TRefPtr<IRHICommandFence> CreateCommandFence() = 0;
	virtual TRefPtr<IRHIRenderTargetView> CreateRenderTargetView(IRHIResource* resource) = 0;
	virtual TRefPtr<IRHIResource> CreateTexture2D(RHITextureFormat format, int32 width, int32 height, RHIResourceStates initialStates, RHIResourceFlags flags) = 0;
	virtual TRefPtr<IRHIDeferredCommandList> CreateDeferredCommandList() = 0;
};