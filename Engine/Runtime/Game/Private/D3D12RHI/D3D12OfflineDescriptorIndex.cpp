// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12OfflineDescriptorIndex.h"

using namespace std;

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex() : Super()
	, PoolIndex(0)
{

}

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex(size_t poolIndex, const vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle) : Super(rev_pool_ref, revision, index, handle)
	, PoolIndex(poolIndex)
{

}

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex(size_t poolIndex, const Super& super) : Super(super)
	, PoolIndex(poolIndex)
{

}

D3D12OfflineDescriptorIndex::~D3D12OfflineDescriptorIndex()
{

}