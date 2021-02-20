// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DeferredCommandList.h"

using namespace std;

D3D12DeferredCommandList::D3D12DeferredCommandList(ID3D12Device* device) : Super()
	, currentAllocatorIndex(0)
{
	this->device = device;

	commandAllocator[0] = move(CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
	commandAllocator[1] = move(CreateCommandAllocator(device, D3D12_COMMAND_LIST_TYPE_DIRECT));
}

D3D12DeferredCommandList::~D3D12DeferredCommandList()
{

}

void D3D12DeferredCommandList::BeginCommand()
{
	HR(commandAllocator[currentAllocatorIndex]->Reset());

	if (!commandList)
	{
		commandList = CreateCommandList(device.Get(), commandAllocator[currentAllocatorIndex].Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	else
	{
		HR(commandList->Reset(commandAllocator[currentAllocatorIndex].Get(), nullptr));
	}

	Super::BeginCommand();
}

void D3D12DeferredCommandList::EndCommand()
{
	MoveAllocatorNext();

	Super::EndCommand();
}

ID3D12GraphicsCommandList4* D3D12DeferredCommandList::CommandList_get()
{
	return commandList.Get();
}

ComPtr<ID3D12CommandAllocator> D3D12DeferredCommandList::CreateCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandAllocator> allocator;
	HR(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator)));
	return move(allocator);
}

ComPtr<ID3D12GraphicsCommandList4> D3D12DeferredCommandList::CreateCommandList(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12GraphicsCommandList4> commandList;
	HR(device->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(&commandList)));
	return move(commandList);
}

void D3D12DeferredCommandList::MoveAllocatorNext()
{
	currentAllocatorIndex = ++currentAllocatorIndex % 2;
}