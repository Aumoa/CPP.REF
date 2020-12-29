// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

class D3D12DescriptorAllocator;

struct D3D12DescriptorIndex
{
public:
	using This = D3D12DescriptorIndex;

private:
	static std::vector<size_t> empty_pool;

	const std::vector<size_t>& rev_pool_ref;
	const size_t revision;

public:
	D3D12DescriptorIndex();
	D3D12DescriptorIndex(const std::vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle);
	~D3D12DescriptorIndex();

	vs_property_get(bool, IsValid);
	bool IsValid_get() const;
		
	const size_t Index;
	const D3D12_CPU_DESCRIPTOR_HANDLE Handle;
};