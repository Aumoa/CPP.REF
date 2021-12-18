// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRHIDeviceChild.h"

interface IRHIDevice;
interface IRHITexture2D;

interface IRHISwapChain : implements IRHIDeviceChild
{
	/// <summary>
	/// Presents a rendered image to the view target.
	/// </summary>
	/// <param name="vSyncLevel"> An integer that specifies how to synchronize presentation of a frame with the vertical blank. </param>
	virtual void Present(int32 vSyncLevel = 0) = 0;

	/// <summary>
	/// Resize all back buffers. All back buffers should be unlocked state.
	/// </summary>
	virtual void ResizeBuffers(int32 width, int32 height) = 0;

	/// <summary>
	/// Get allocated buffer.
	/// </summary>
	virtual IRHITexture2D* GetBuffer(int32 index) = 0;

	/// <summary>
	/// Get current back buffer index.
	/// </summary>
	virtual int32 GetCurrentBackBufferIndex() = 0;
};