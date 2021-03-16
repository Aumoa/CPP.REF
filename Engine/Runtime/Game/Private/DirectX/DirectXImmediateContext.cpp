// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXImmediateContext.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXCommandQueue.h"

DirectXImmediateContext::DirectXImmediateContext(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, D3D12_COMMAND_LIST_TYPE type) : Super(deviceBundle, type)
	, commandQueue(commandQueue)
{

}

DirectXImmediateContext::~DirectXImmediateContext()
{

}

void DirectXImmediateContext::EndDraw()
{
	Super::EndDraw();

	commandQueue->ExecuteCommandLists(this);
}