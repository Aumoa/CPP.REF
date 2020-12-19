// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DescriptorAllocator.h"

#include "D3D12DescriptorIndex.h"
#include "Logging/LogMacros.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game::D3D12RHI;
using namespace std;

D3D12DescriptorAllocator::D3D12DescriptorAllocator(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptors, bool bIsOffline, bool bThreadSafe) : Super()
	, Type(type)
	, Count(numDescriptors)
	, DeviceRef(device)
	, IncrementSize(device->GetDescriptorHandleIncrementSize(type))

	, descriptorHeap(nullptr)
	, bOffline(bIsOffline)
	, bThreadSafe(bThreadSafe)
{
	D3D12_DESCRIPTOR_HEAP_DESC desc;
	desc.Type = type;
	desc.NumDescriptors = (UINT)numDescriptors;
	desc.Flags = bIsOffline ? D3D12_DESCRIPTOR_HEAP_FLAG_NONE : D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	HR(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

	InitializePool();
}

D3D12DescriptorAllocator::~D3D12DescriptorAllocator()
{

}

D3D12DescriptorIndex D3D12DescriptorAllocator::Alloc()
{
	unique_lock<mutex> lock_locker;

	if (bThreadSafe)
	{
		lock_locker = move(unique_lock(locker));
	}

	if (descriptor_pool.size() == 0)
	{
		SE_LOG(LogD3D12RHI, Error, L"There is no consumable pool instance.");
		return { };
	}

	size_t idx = descriptor_pool.front();
	size_t& rev = rev_pool[idx];

	D3D12_CPU_DESCRIPTOR_HANDLE startAddr = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	startAddr.ptr += IncrementSize * idx;

	descriptor_pool.pop();
	return D3D12DescriptorIndex(rev_pool, ++rev, idx, startAddr);
}

void D3D12DescriptorAllocator::Free(const D3D12DescriptorIndex& index)
{
	unique_lock<mutex> lock_locker;

	if (bThreadSafe)
	{
		lock_locker = move(unique_lock(locker));
	}

	if (index.IsValid)
	{
		descriptor_pool.push(index.Index);
		++rev_pool.at(index.Index);
	}
}

size_t D3D12DescriptorAllocator::Slack_get() const
{
	return descriptor_pool.size();
}

bool D3D12DescriptorAllocator::IsOffline_get() const
{
	return bOffline;
}

bool D3D12DescriptorAllocator::IsThreadSafe_get() const
{
	return bThreadSafe;
}

void D3D12DescriptorAllocator::InitializePool()
{
	rev_pool.resize(Count);

	for (uint32 i = 0; i < Count; ++i)
	{
		descriptor_pool.push(i);
		rev_pool[i] = 0;
	}
}