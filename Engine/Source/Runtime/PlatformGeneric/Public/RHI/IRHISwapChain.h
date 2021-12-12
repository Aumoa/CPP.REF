// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIDevice;
interface IRHITexture2D;

interface IRHISwapChain : implements IRHIDeviceChild
{
	virtual void Present(int32 vSyncLevel = 0) = 0;
	virtual void ResizeBuffers(int32 Width, int32 Height) = 0;
	virtual std::shared_ptr<IRHITexture2D> GetBuffer(int32 Index) = 0;
	virtual int32 GetCurrentBackBufferIndex() = 0;
};