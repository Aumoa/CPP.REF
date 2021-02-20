// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12DynamicBufferAllocator.h"

#include "Engine.h"
#include "D3D12DeviceBundle.h"
#include "Logging/LogMacros.h"

D3D12DynamicBufferAllocator::D3D12DynamicBufferAllocator(size_t inCapacity, size_t inNodeSize) : Super()
	, capacity(inCapacity)
	, nodeSize(inNodeSize)
	, fullSize(inNodeSize* inCapacity)
	, addr(nullptr)
{
	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = (uint64)fullSize;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	D3D12_HEAP_PROPERTIES heapProp{ };
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	HR(Cast<D3D12DeviceBundle>(GEngine.DeviceBundle)->Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer)));

	for (size_t i = 0; i < inCapacity; ++i)
	{
		myqueue.emplace(i);
	}

	this->bufferDesc.Width = (uint64)fullSize;
	this->bufferDesc.Height = 1;
	this->bufferDesc.DepthOrArraySize = 1;
	this->bufferDesc.Format = ERHITextureFormat::Unknown;

	HR(buffer->Map(0, nullptr, &addr));
}

D3D12DynamicBufferAllocator::~D3D12DynamicBufferAllocator()
{

}

RHIResourceDesc D3D12DynamicBufferAllocator::GetBufferDesc() const
{
	return bufferDesc;
}

uint64 D3D12DynamicBufferAllocator::GetVirtualAddress(size_t inNodeIdx) const
{
	return buffer->GetGPUVirtualAddress() + (uint64)(nodeSize * inNodeIdx);
}

void* D3D12DynamicBufferAllocator::GetMappingAddress(size_t inNodeIdx) const
{
	return (void*)((char*)addr + (uint64)(nodeSize * inNodeIdx));
}

size_t D3D12DynamicBufferAllocator::Alloc()
{
	if (Slack == 0)
	{
		SE_LOG(LogEngine, Error, L"Cannot create dynamic buffer from allocator.");
		return -1;
	}

	size_t myidx = myqueue.front();
	myqueue.pop();

	return myidx;
}

void D3D12DynamicBufferAllocator::Free(size_t inNodeIdx)
{
	myqueue.emplace(inNodeIdx);
}

size_t D3D12DynamicBufferAllocator::Slack_get() const
{
	return myqueue.size();
}