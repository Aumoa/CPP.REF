// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.RenderCore:RHICommandQueue;

import SC.Runtime.Core;
import SC.Runtime.RenderCore.Internal;
import :ComPtr;
import :RHIEnums;
import :RHIDeviceChild;
import std.core;

export class RHIDevice;
export class RHIDeviceContext;

using namespace std;

/// <summary>
/// Provides methods for submitting command lists that written by device context, synchronizing device context execution.
/// </summary>
export class RHICommandQueue : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	ComPtr<ID3D12CommandQueue> _queue;
	ComPtr<ID3D12Fence> _fence;
	uint64 _signalNumber = 0;
	EventHandle* _fenceEvent = nullptr;

public:
	/// <summary>
	/// Initialize new <see cref="RHICommandQueue"/> instance.
	/// </summary>
	/// <param name="device"> The logical device. </param>
	/// <param name="commandType"> Specify command type for usage. </param>
	RHICommandQueue(RHIDevice* device, ERHICommandType commandType = ERHICommandType::Direct);
	~RHICommandQueue() override;

	/// <summary>
	/// Signal the last committed command list for reference from CPU commands and waiting.
	/// </summary>
	/// <returns> The signal number. </returns>
	uint64 Signal();

	/// <summary>
	/// Wait for commands executed that represented by signal number.
	/// </summary>
	/// <param name="signalNumber"> The signal number. </param>
	void WaitSignal(uint64 signalNumber);

	/// <summary>
	/// Wait for commands executed that represented by last signaled number.
	/// </summary>
	void WaitLastSignal();

	/// <summary>
	/// Execute a device context.
	/// </summary>
	void ExecuteDeviceContext(RHIDeviceContext* deviceContext)
	{
		RHIDeviceContext* deviceContextsSpan[] = { deviceContext };
		ExecuteDeviceContexts(span(deviceContextsSpan));
	}

	/// <summary>
	/// Execute multiple device contexts.
	/// </summary>
	void ExecuteDeviceContexts(span<RHIDeviceContext*> deviceContexts);

	/// <summary>
	/// Execute multiple device contexts.
	/// </summary>
	template<class... TDeviceContexts>
	void ExecuteDeviceContext(TDeviceContexts&&... deviceContexts)
	{
		RHIDeviceContext* deviceContextsSpan[] = { deviceContexts... };
		ExecuteDeviceContexts(span(deviceContextsSpan));
	}

public /*internal*/:
	ID3D12CommandQueue* GetCommandQueue() const { return _queue.Get(); }
};