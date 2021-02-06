// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DescriptorIndex.h"

using namespace std;

vector<size_t> D3D12DescriptorIndex::empty_pool;

D3D12DescriptorIndex::D3D12DescriptorIndex()
	: rev_pool_ref(empty_pool)
	, revision(0)

	, Index(0)
	, Handle{ }
	, HandleGPU{ }
	, IsOnline(false)
{

}

D3D12DescriptorIndex::D3D12DescriptorIndex(const vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle)
	: rev_pool_ref(rev_pool_ref)
	, revision(revision)

	, Index(index)
	, Handle(handle)
	, HandleGPU{ }
	, IsOnline(false)
{

}

D3D12DescriptorIndex::D3D12DescriptorIndex(const vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle, D3D12_GPU_DESCRIPTOR_HANDLE handleGpu)
	: rev_pool_ref(rev_pool_ref)
	, revision(revision)

	, Index(index)
	, Handle(handle)
	, HandleGPU(handleGpu)
	, IsOnline(true)
{

}

D3D12DescriptorIndex::~D3D12DescriptorIndex()
{

}

bool D3D12DescriptorIndex::IsValid_get() const
{
	if (rev_pool_ref.size() <= Index)
	{
		return false;
	}

	if (rev_pool_ref[Index] != revision)
	{
		return false;
	}

	return true;
}