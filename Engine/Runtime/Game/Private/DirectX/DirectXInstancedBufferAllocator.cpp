// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXInstancedBufferAllocator.h"

#include "DirectXCommon.h"

DirectXInstancedBufferAllocator::DirectXInstancedBufferAllocator(DirectXDeviceBundle* deviceBundle, bool bDynamic) : Super(deviceBundle)
	, bDynamic(bDynamic)
	, count(0)
	, capacity(0)
{

}

DirectXInstancedBufferAllocator::~DirectXInstancedBufferAllocator()
{

}