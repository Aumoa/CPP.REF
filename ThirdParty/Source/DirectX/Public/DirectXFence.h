// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"
#include "Threading/TaskCompletionSource.h"

class DIRECTX_API SDirectXFence : public SDirectXDeviceChild, implements IRHIFence
{
	GENERATED_BODY(SDirectXFence)

public:
	ComPtr<ID3D12Fence> pFence;
	std::atomic<uint64> FenceValue = 0;
	
private:
	struct WaitContext
	{
		HANDLE hEvent = nullptr;
		HANDLE hWait = nullptr;
		TaskCompletionSource<> TCS;
		std::atomic<bool> bReady = false;
	};

	std::mutex _lock;
	std::vector<std::unique_ptr<WaitContext>> _contexts;

public:
	SDirectXFence(IRHIDevice* Owner, ComPtr<ID3D12Fence> pFence);

	using Super::Dispose;

	virtual Task<> SetEventOnCompletion(uint64 fenceValue, std::optional<std::chrono::milliseconds> timeout) override;
	virtual uint64 GetCompletedValue() override;

protected:
	virtual void Dispose(bool bDisposing) override;

private:
	static VOID CALLBACK OnConnect(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
};