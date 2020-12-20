// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12ImmediateCommandList.h"

using namespace SC::Runtime::Game::D3D12RHI;
using namespace std;

D3D12ImmediateCommandList::D3D12ImmediateCommandList(ID3D12Device* device) : Super()
	, currentAllocatorIndex(0)
{
	this->device = device;

	D3D12_COMMAND_QUEUE_DESC desc{ };
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

	commandAllocator[0] = move(CreateCommandAllocator(device, desc.Type));
	commandAllocator[1] = move(CreateCommandAllocator(device, desc.Type));
}

D3D12ImmediateCommandList::~D3D12ImmediateCommandList()
{

}

void D3D12ImmediateCommandList::BeginCommand()
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
}

void D3D12ImmediateCommandList::EndCommand()
{
	Super::EndCommand();
	MoveAllocatorNext();
}

void D3D12ImmediateCommandList::Flush()
{
	ID3D12CommandList* commandListRef = commandList.Get();
	commandQueue->ExecuteCommandLists(1, &commandListRef);
}

ID3D12GraphicsCommandList* D3D12ImmediateCommandList::CommandList_get()
{
	return commandList.Get();
}

ID3D12CommandQueue* D3D12ImmediateCommandList::CommandQueue_get() const
{
	return commandQueue.Get();
}

ComPtr<ID3D12CommandAllocator> D3D12ImmediateCommandList::CreateCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12CommandAllocator> allocator;
	HR(device->CreateCommandAllocator(type, IID_PPV_ARGS(&allocator)));
	return move(allocator);
}

ComPtr<ID3D12GraphicsCommandList> D3D12ImmediateCommandList::CreateCommandList(ID3D12Device* device, ID3D12CommandAllocator* allocator, D3D12_COMMAND_LIST_TYPE type)
{
	ComPtr<ID3D12GraphicsCommandList> commandList;
	HR(device->CreateCommandList(0, type, allocator, nullptr, IID_PPV_ARGS(&commandList)));
	return move(commandList);
}

void D3D12ImmediateCommandList::MoveAllocatorNext()
{
	currentAllocatorIndex = ++currentAllocatorIndex % 2;
}