// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXCommandQueue.h"

#include "DirectXMinimal.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXCommandQueue::DirectXCommandQueue(DirectXDeviceBundle* deviceBundle) : DirectXCommandQueue(deviceBundle, D3D12_COMMAND_LIST_TYPE_DIRECT)
{

}

DirectXCommandQueue::DirectXCommandQueue(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type) : Super()
{
	ID3D12Device5* device = deviceBundle->GetDevice();
	
	D3D12_COMMAND_QUEUE_DESC desc = { };
	desc.Type = type;
	HR(device->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue)));

	SetDeviceChildPtr(commandQueue.Get(), deviceBundle);
}

DirectXCommandQueue::~DirectXCommandQueue()
{

}