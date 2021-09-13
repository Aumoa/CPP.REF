// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include "ComPtr.h"

struct IDXGISwapChain4;
struct IWindowView;
class SRHITexture2D;
class SRHICommandQueue;

/// <summary>
/// Represents interface implements one or more surfaces for storing rendered data before presenting it to an output.
/// </summary>
class RENDERCORE_API SRHISwapChain : virtual public SRHIDeviceChild
{
	GENERATED_BODY(SRHISwapChain)

private:
	ComPtr<IDXGISwapChain4> _swapChain;
	SRHITexture2D* _buffers[3] = {};

public:
	/// <summary>
	/// Initialize new <see cref="SRHISwapChain"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="view"> The render target view. </param>
	/// <param name="queue"> Specify command queue that swap chain be presenting. </param>
	SRHISwapChain(SRHIDevice* device, IWindowView* view, SRHICommandQueue* queue);
	~SRHISwapChain() override;

	/// <summary>
	/// Presents a rendered image to the view target.
	/// </summary>
	/// <param name="vSyncLevel"> An integer that specifies how to synchronize presentation of a frame with the vertical blank. </param>
	void Present(uint8 vSyncLevel = 0);

	/// <summary>
	/// Resize all back buffers. All back buffers should be unlocked state.
	/// </summary>
	void ResizeBuffers(int32 width, int32 height);

	/// <summary>
	/// Get allocated buffer.
	/// </summary>
	SRHITexture2D* GetBuffer(int32 index) const;

	/// <summary>
	/// Get current back buffer index.
	/// </summary>
	int32 GetCurrentBackBufferIndex() const;
};