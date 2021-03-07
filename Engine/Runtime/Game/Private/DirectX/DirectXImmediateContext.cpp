// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXImmediateContext.h"

#include "DirectX/DirectXCommandQueue.h"

DirectXImmediateContext::DirectXImmediateContext(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, D3D12_COMMAND_LIST_TYPE type) : Super(deviceBundle, type)
{
	this->commandQueue = commandQueue;
}

DirectXImmediateContext::~DirectXImmediateContext()
{

}