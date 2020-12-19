// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12CommandList.h"

#include "D3D12RenderTargetView.h"

using namespace SC::Runtime::Game::D3D12RHI;
using namespace SC::Runtime::Game::RHI;
using namespace std;

D3D12CommandList::D3D12CommandList()
{

}

D3D12CommandList::~D3D12CommandList()
{

}

void D3D12CommandList::OMSetRenderTargets(size_t count, IRHIRenderTargetView* rtv[])
{
	vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles(count);
	for (size_t i = 0; i < count; ++i)
	{
		auto rtv_cast = dynamic_cast<D3D12RenderTargetView*>(rtv[i]);
		handles[i] = rtv_cast->Handle;
	}

	CommandList->OMSetRenderTargets((UINT)count, handles.data(), FALSE, nullptr);
}