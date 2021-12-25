// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12CommandQueue.h"
#include "Threading/EventHandle.h"

GENERATE_BODY(SD3D12CommandQueue);

SD3D12CommandQueue::SD3D12CommandQueue(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12CommandQueue> Queue, ComPtr<ID3D12Fence> Fence) : Super(InFactory, InDevice)
	, Queue(std::move(Queue))
	, Fence(std::move(Fence))
{
	Event = gcnew SEventHandle();
}

void SD3D12CommandQueue::End()
{
	Super::End();
	ExecuteCommandList(this);
}

uint64 SD3D12CommandQueue::ExecuteCommandLists(std::span<IRHIDeviceContext*> DeviceContexts, bool bSignal)
{
	std::vector<ID3D12CommandList*> CommandLists;
	CommandLists.reserve(DeviceContexts.size());

	++FenceValue;

	for (size_t i = 0; i < DeviceContexts.size(); ++i)
	{
		auto s_DeviceContext = Cast<SD3D12CommandList>(DeviceContexts[i]);
		if (s_DeviceContext)
		{
			if (auto CommandList = s_DeviceContext->Get<ID3D12CommandList>(); CommandList)
			{
				CommandLists.emplace_back(CommandList);

				std::vector<SObject*> Objects = s_DeviceContext->ClearPendingObjects();

				LockedGarbages Garbage;
				Garbage.MarkedValue = FenceValue;
				Garbage.Objects.reserve(Objects.size());

				for (auto& Obj : Objects)
				{
					Garbage.Objects.emplace_back(Cast<IDisposable>(Obj));
				}

				PendingGarbages.emplace(std::move(Garbage));
			}
		}
	}

	if (CommandLists.size())
	{
		Queue->ExecuteCommandLists((UINT)CommandLists.size(), CommandLists.data());
	}

	if (bSignal)
	{
		Queue->Signal(Fence.Get(), FenceValue);
		return FenceValue;
	}
	else
	{
		return --FenceValue;
	}
}

uint64 SD3D12CommandQueue::GetFenceValue()
{
	return FenceValue;
}

uint64 SD3D12CommandQueue::GetCompletedValue()
{
	return Fence->GetCompletedValue();
}

void SD3D12CommandQueue::Collect()
{
	while (!PendingGarbages.empty())
	{
		auto& Front = PendingGarbages.front();
		if (Front.MarkedValue <= Fence->GetCompletedValue())
		{
			for (size_t i = 0; i < Front.Objects.size(); ++i)
			{
				Front.Objects[i]->Dispose();
			}

			PendingGarbages.pop();
		}
		else
		{
			break;
		}
	}
}

void SD3D12CommandQueue::WaitCompleted()
{
	if (Fence->GetCompletedValue() < FenceValue)
	{
		Fence->SetEventOnCompletion(FenceValue, Event->GetHandle());
		Event->Wait();
	}
}

void SD3D12CommandQueue::Dispose(bool bDisposing)
{
	WaitCompleted();

	Queue.Reset();
	Fence.Reset();

	if (bDisposing)
	{
		std::queue<LockedGarbages> Swap;
		std::swap(Swap, PendingGarbages);
		Event = nullptr;
	}

	Super::Dispose(bDisposing);
}