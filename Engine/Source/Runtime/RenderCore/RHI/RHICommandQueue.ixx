// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHICommandQueue;

import SC.Runtime.Core;
import SC.Runtime.Core.Internal;
import :ComPtr;
import :RHICommandType;
import :RHIDeviceChild;

export class RHIDevice;

/// <summary>
/// Provides methods for submitting command lists that written by device context, synchronizing device context execution.
/// </summary>
export class RHICommandQueue : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	ComPtr<ID3D12CommandQueue> _queue;

public:
	/// <summary>
	/// Initialize new <see cref="RHICommandQueue"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="commandType"> Specify command type for usage. </param>
	RHICommandQueue(RHIDevice* device, ERHICommandType commandType = ERHICommandType::Direct);
	~RHICommandQueue() override;

public /*internal*/:
	ID3D12CommandQueue* GetCommandQueue() const { return _queue.Get(); }
};