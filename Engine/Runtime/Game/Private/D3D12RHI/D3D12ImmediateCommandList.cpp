// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12ImmediateCommandList.h"

using namespace SC::Runtime::Game::D3D12RHI;
using namespace std;

D3D12ImmediateCommandList::D3D12ImmediateCommandList(ID3D12Device* device) : Super(device)
{
	this->device = device;

	D3D12_COMMAND_QUEUE_DESC desc{ };
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));
}

D3D12ImmediateCommandList::~D3D12ImmediateCommandList()
{

}

void D3D12ImmediateCommandList::Flush()
{
	ID3D12CommandList* commandListRef = CommandList;
	commandQueue->ExecuteCommandLists(1, &commandListRef);
}

ID3D12CommandQueue* D3D12ImmediateCommandList::CommandQueue_get() const
{
	return commandQueue.Get();
}