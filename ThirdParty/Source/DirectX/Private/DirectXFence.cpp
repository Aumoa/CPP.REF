// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXFence.h"

using namespace ::libty;

SDirectXFence::SDirectXFence(IRHIDevice* Owner, ComPtr<ID3D12Fence> pFence)
	: Super(Owner)
	, pFence(std::move(pFence))
{
}

Task<> SDirectXFence::SetEventOnCompletion(uint64 fenceValue, std::optional<std::chrono::milliseconds> timeout)
{
	if (GetCompletedValue() < fenceValue)
	{
		std::unique_lock lock(_lock);
		WaitContext* pContext = nullptr;
		for (size_t i = 0; i < _contexts.size(); ++i)
		{
			if (_contexts[i]->bReady)
			{
				pContext = _contexts[i].get();
				break;
			}
		}

		if (pContext == nullptr)
		{
			pContext = _contexts.emplace_back(std::make_unique<WaitContext>()).get();
			pContext->hEvent = CreateEventExW(nullptr, nullptr, 0, GENERIC_ALL);
		}

		pContext->TCS = TaskCompletionSource<>::Create();
		pContext->bReady = false;

		auto task = pContext->TCS.GetTask();
		lock.unlock();

		HR(pFence->SetEventOnCompletion(FenceValue, pContext->hEvent));
		BOOL bResult = RegisterWaitForSingleObject(
			&pContext->hWait,
			pContext->hEvent,
			&SDirectXFence::OnConnect,
			pContext,
			(ULONG)timeout.value_or(std::chrono::milliseconds(INFINITE)).count(),
			WT_EXECUTEONLYONCE);
		if (!bResult)
		{
			pContext->TCS.SetException(std::make_exception_ptr(InvalidOperationException(TEXT("Failed to create callback procedure for waiting event."))));
			pContext->bReady = true;
		}

		return task;
	}

	return Task<>::CompletedTask();
}

uint64 SDirectXFence::GetCompletedValue()
{
	return pFence->GetCompletedValue();
}

void SDirectXFence::Dispose(bool bDisposing)
{
	for (auto& ctx : _contexts)
	{
		if (!ctx)
		{
			continue;
		}

		ctx->TCS.GetTask().Wait();

		if (ctx->hEvent)
		{
			CloseHandle(ctx->hEvent);
			ctx->hEvent = nullptr;
		}
	}

	pFence.Reset();
	Super::Dispose(bDisposing);
}

VOID CALLBACK SDirectXFence::OnConnect(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	WaitContext* pContext = reinterpret_cast<WaitContext*>(lpParameter);
	if (TimerOrWaitFired)
	{
		pContext->TCS.SetException(std::make_exception_ptr(TaskCanceledException()));
	}
	else
	{
		pContext->TCS.SetResult();
	}

	//CloseHandle(pContext->hWait);
	pContext->hWait = nullptr;
	pContext->bReady = true;
}