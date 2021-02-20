// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Minimal.h"

#include "Logging/LogVerbosity.h"

using namespace std;

LogCategoryBase LogD3D12RHI(ELogVerbosity::Verbose, nameof(LogD3D12RHI));

CD3DX12_RESOURCE_BARRIER::CD3DX12_RESOURCE_BARRIER() : D3D12_RESOURCE_BARRIER{ }
{

}

CD3DX12_RESOURCE_BARRIER& CD3DX12_RESOURCE_BARRIER::SwapTransition()
{
	swap(Transition.StateBefore, Transition.StateAfter);
	return *this;
}

CD3DX12_RESOURCE_BARRIER CD3DX12_RESOURCE_BARRIER::TransitionBarrier(ID3D12Resource* inResource, D3D12_RESOURCE_STATES inBefore, D3D12_RESOURCE_STATES inAfter, uint32 inSubresourceIdx)
{
	CD3DX12_RESOURCE_BARRIER barrier = { };
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.pResource = inResource;
	barrier.Transition.StateBefore = inBefore;
	barrier.Transition.StateAfter = inAfter;
	barrier.Transition.Subresource = inSubresourceIdx;
	return barrier;
}