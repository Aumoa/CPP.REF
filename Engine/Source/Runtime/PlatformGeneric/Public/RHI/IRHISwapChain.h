// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIDevice;
interface IRHITexture2D;

interface IRHISwapChain : implements IRHIDeviceChild
{
	GENERATED_INTERFACE_BODY(IRHISwapChain)

	virtual void Present(int32 vSyncLevel = 0) = 0;
	virtual void ResizeBuffers(int32 width, int32 height) = 0;
	virtual IRHITexture2D* GetBuffer(int32 index) = 0;
	virtual int32 GetCurrentBackBufferIndex() = 0;
};