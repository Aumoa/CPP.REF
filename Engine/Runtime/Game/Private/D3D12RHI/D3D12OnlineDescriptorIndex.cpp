// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12OnlineDescriptorIndex.h"

#include "D3D12OnlineDescriptorManager.h"

using namespace std;

D3D12OnlineDescriptorIndex::D3D12OnlineDescriptorIndex() : Super()
	, pTable(nullptr)
	, pManager(nullptr)
{

}

D3D12OnlineDescriptorIndex::D3D12OnlineDescriptorIndex(ID3D12DescriptorHeap* inTable, D3D12OnlineDescriptorManager* inManager, const vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle) : Super(rev_pool_ref, revision, index, handle)
	, pTable(inTable)
	, pManager(inManager)
{

}

D3D12OnlineDescriptorIndex::D3D12OnlineDescriptorIndex(ID3D12DescriptorHeap* inTable, D3D12OnlineDescriptorManager* inManager, const Super& super) : Super(super)
	, pTable(inTable)
	, pManager(inManager)
{

}

void D3D12OnlineDescriptorIndex::Free()
{
	if (pManager != nullptr)
	{
		pManager->Free(*this);
	}
}

D3D12OnlineDescriptorIndex::~D3D12OnlineDescriptorIndex()
{

}