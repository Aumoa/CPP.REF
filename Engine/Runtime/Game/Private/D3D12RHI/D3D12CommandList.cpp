// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"

#include "D3D12RenderTargetView.h"
#include "D3D12Resource.h"

using namespace std;

D3D12CommandList::D3D12CommandList() : Super()
	, bHasBegunCommand(false)
{

}

D3D12CommandList::~D3D12CommandList()
{

}

void D3D12CommandList::BeginCommand()
{
	bHasBegunCommand = true;
}

void D3D12CommandList::EndCommand()
{
	ConsumePendingDeferredCommands();
	HR(CommandList->Close());
	bHasBegunCommand = false;
}

void D3D12CommandList::OMSetRenderTargets(size_t count, IRHIRenderTargetView* rtv[])
{
	ConsumePendingDeferredCommands();

	vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(count);
	for (size_t i = 0; i < count; ++i)
	{
		auto rtv_cast = dynamic_cast<D3D12RenderTargetView*>(rtv[i]);
		handles[i] = rtv_cast->Handle;
	}

	CommandList->OMSetRenderTargets((UINT)count, handles.data(), FALSE, nullptr);
}

void D3D12CommandList::ClearRenderTargetView(IRHIRenderTargetView* rtv)
{
	ConsumePendingDeferredCommands();

	constexpr FLOAT ClearColor[4] = { 0, 0, 0, 0 };
	auto rtv_cast = Cast<D3D12RenderTargetView>(rtv);
	CommandList->ClearRenderTargetView(rtv_cast->Handle, ClearColor, 0, nullptr);
}

void D3D12CommandList::ResourceTransition(IRHIResource* resource, RHIResourceStates beforeState, RHIResourceStates afterState, size_t subresourceIndex)
{
	D3D12_RESOURCE_BARRIER barrier{ };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = Cast<D3D12Resource>(resource)->Resource;
	barrier.Transition.StateBefore = ToD3D12(beforeState);
	barrier.Transition.StateAfter = ToD3D12(afterState);
	barrier.Transition.Subresource = (uint32)subresourceIndex;

	pendingBarriers.emplace_back(barrier);
}

void D3D12CommandList::CopyResource(IRHIResource* target, IRHIResource* source)
{
	ConsumePendingDeferredCommands();

	CommandList->CopyResource(Cast<D3D12Resource>(target)->Resource, Cast<D3D12Resource>(source)->Resource);
}

bool D3D12CommandList::HasBegunCommand_get() const
{
	return bHasBegunCommand;
}

void D3D12CommandList::ConsumePendingDeferredCommands()
{
	{
		vector<D3D12_RESOURCE_BARRIER> barrier_swap;
		swap(barrier_swap, pendingBarriers);

		if (!barrier_swap.empty())
		{
			CommandList->ResourceBarrier((UINT)barrier_swap.size(), barrier_swap.data());
		}
	}
}