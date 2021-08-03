// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHIDeviceChild.h"
#include <atomic>
#include <queue>
#include <span>
#include "ComPtr.h"
#include "RHIEnums.h"
#include "Threading/EventHandle.h"

struct IUnknown;
struct ID3D12CommandQueue;
struct ID3D12Fence;
class RHIDeviceContext;

/// <summary>
/// Provides methods for submitting command lists that written by device context, synchronizing device context execution.
/// </summary>
class RENDERCORE_API RHICommandQueue : public RHIDeviceChild
{
public:
	using Super = RHIDeviceChild;

private:
	struct GarbageItem
	{
		int32 TypeIndex = 0;
		uint64 FenceValue = 0;
		union
		{
			IUnknown* IsUnknown;
			Object* IsObject;
		};
	};

private:
	ComPtr<ID3D12CommandQueue> _queue;
	ComPtr<ID3D12Fence> _fence;
	std::atomic<uint64> _signalNumber = 0;
	EventHandle* _fenceEvent = nullptr;
	std::queue<GarbageItem> _gcobjects;

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
	uint64 ExecuteDeviceContext(RHIDeviceContext* deviceContext)
	{
		RHIDeviceContext* deviceContextsSpan[] = { deviceContext };
		return ExecuteDeviceContexts(std::span(deviceContextsSpan));
	}

	/// <summary>
	/// Execute multiple device contexts.
	/// </summary>
	uint64 ExecuteDeviceContexts(std::span<RHIDeviceContext*> deviceContexts);

	/// <summary>
	/// Execute multiple device contexts.
	/// </summary>
	template<class... TDeviceContexts>
	uint64 ExecuteDeviceContext(TDeviceContexts&&... deviceContexts)
	{
		RHIDeviceContext* deviceContextsSpan[] = { deviceContexts... };
		return ExecuteDeviceContexts(span(deviceContextsSpan));
	}

	/// <summary>
	/// Collect all registered garbages.
	/// </summary>
	/// <returns> Count of collected items. </returns>
	int32 Collect();

	/// <summary>
	/// Add garbage object. Object will destroy at Collect() called and outer will change to this object.
	/// </summary>
	void AddGarbageObject(uint64 fenceValue, Object* object);

public /*internal*/:
	ID3D12CommandQueue* GetCommandQueue() const { return _queue.Get(); }
	void AddGarbageObject(uint64 fenceValue, IUnknown* unknown);
};