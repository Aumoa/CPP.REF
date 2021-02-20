// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12ImmediateCommandList.h"

#include "Logging/LogMacros.h"

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

void D3D12ImmediateCommandList::ExecuteCommandList(IRHIDeferredCommandList* deferredCommandList)
{
	if (HasBegunCommand)
	{
		SE_LOG(LogD3D12RHI, Warning, L"ExecuteCommandList function must be called after command list is closed. Execute command will run without current written commands.");
	}

	ID3D12CommandList* commandListRef = Cast<D3D12DeferredCommandList>(deferredCommandList)->CommandList;
	commandQueue->ExecuteCommandLists(1, &commandListRef);
}

void D3D12ImmediateCommandList::Flush()
{
	ExecuteCommandList(this);
}

ID3D12CommandQueue* D3D12ImmediateCommandList::CommandQueue_get() const
{
	return commandQueue.Get();
}