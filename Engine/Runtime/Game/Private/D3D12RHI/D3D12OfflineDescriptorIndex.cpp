// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12OfflineDescriptorIndex.h"

#include "D3D12OfflineDescriptorManager.h"

using namespace std;

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex() : Super()
	, PoolIndex(0)
	, pManager(nullptr)
{


}

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex(size_t poolIndex, D3D12OfflineDescriptorManager* inManager, const vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle) : Super(rev_pool_ref, revision, index, handle)
	, PoolIndex(poolIndex)
	, pManager(inManager)
{

}

D3D12OfflineDescriptorIndex::D3D12OfflineDescriptorIndex(size_t poolIndex, D3D12OfflineDescriptorManager* inManager, const Super& super) : Super(super)
	, PoolIndex(poolIndex)
	, pManager(inManager)
{

}

void D3D12OfflineDescriptorIndex::Free()
{
	if (pManager != nullptr)
	{
		pManager->Free(*this);
	}
}

D3D12OfflineDescriptorIndex::~D3D12OfflineDescriptorIndex()
{

}