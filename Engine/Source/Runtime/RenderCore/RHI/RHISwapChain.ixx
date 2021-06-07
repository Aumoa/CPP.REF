// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHISwapChain;

import SC.Runtime.Core;
import SC.Runtime.RenderCore.Internal;
import :ComPtr;
import :RHIDeviceChild;

export struct IWindowView;
export class RHIDevice;
export class RHICommandQueue;
export class RHITexture2D;

/// <summary>
/// Represents interface implements one or more surfaces for storing rendered data before presenting it to an output.
/// </summary>
export class RHISwapChain : virtual public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	ComPtr<IDXGISwapChain4> _swapChain;
	RHITexture2D* _buffers[3] = {};

public:
	/// <summary>
	/// Initialize new <see cref="RHISwapChain"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="view"> The render target view. </param>
	/// <param name="queue"> Specify command queue that swap chain be presenting. </param>
	RHISwapChain(RHIDevice* device, IWindowView* view, RHICommandQueue* queue);
	~RHISwapChain() override;

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
	RHITexture2D* GetBuffer(int32 index) const;
};