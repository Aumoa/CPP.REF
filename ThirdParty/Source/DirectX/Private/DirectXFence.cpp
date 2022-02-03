// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXFence.h"
#include "Threading/EventHandle.h"

GENERATE_BODY(SDirectXFence);

SDirectXFence::SDirectXFence(IRHIDevice* Owner, ComPtr<ID3D12Fence> pFence)
	: Super(Owner)
	, pFence(std::move(pFence))
{
	hFenceEvent = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
}

void SDirectXFence::Wait()
{
	if (IsReady())
	{
		HR(pFence->SetEventOnCompletion(FenceValue, hFenceEvent));
		WaitForSingleObject(hFenceEvent, INFINITE);
	}
}

bool SDirectXFence::IsReady()
{
	return pFence->GetCompletedValue() == FenceValue;
}

void SDirectXFence::Dispose(bool bDisposing)
{
	if (hFenceEvent)
	{
		CloseHandle(hFenceEvent);
		hFenceEvent = nullptr;
	}

	pFence.Reset();
	Super::Dispose(bDisposing);
}