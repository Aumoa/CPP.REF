// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "DirectXDeviceChild.h"

class SEventHandle;

class DIRECTX_API SDirectXFence : public SDirectXDeviceChild, implements IRHIFence
{
	GENERATED_BODY(SDirectXFence)

public:
	ComPtr<ID3D12Fence> pFence;
	std::atomic<uint64> FenceValue = 0;
	
private:
	HANDLE hFenceEvent = nullptr;

public:
	SDirectXFence(IRHIDevice* Owner, ComPtr<ID3D12Fence> pFence);

	using Super::Dispose;

	virtual void Wait() override;
	virtual bool IsReady() override;

protected:
	virtual void Dispose(bool bDisposing) override;
};