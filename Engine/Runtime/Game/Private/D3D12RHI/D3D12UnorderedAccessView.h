// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIUnorderedAccessView.h"

#include "D3D12Minimal.h"
#include "D3D12OfflineDescriptorIndex.h"

struct D3D12OfflineDescriptorIndex;

class D3D12UnorderedAccessView : virtual public Object, virtual public IRHIUnorderedAccessView
{
public:
	using Super = Object;

public:
	D3D12UnorderedAccessView();
	~D3D12UnorderedAccessView() override;

	vs_property_get_pure(D3D12_CPU_DESCRIPTOR_HANDLE, HandleBase);
};

class D3D12SingleUnorderedAccessViewNode : public D3D12UnorderedAccessView
{
public:
	using Super = D3D12UnorderedAccessView;

private:
	D3D12OfflineDescriptorIndex handle;

public:
	D3D12SingleUnorderedAccessViewNode(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle);
	~D3D12SingleUnorderedAccessViewNode() override;

	size_t Count_get() const override;
	D3D12_CPU_DESCRIPTOR_HANDLE HandleBase_get() const;
};

class D3D12IndependentUnorderedAccessView : public D3D12UnorderedAccessView
{
public:
	using Super = D3D12UnorderedAccessView;

private:
	ID3D12Device* const device;
	size_t const numSRVs;
	ComPtr<ID3D12DescriptorHeap> heap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE handleBase;
	uint32 increment;

public:
	D3D12IndependentUnorderedAccessView(ID3D12Device* device, size_t numSRVs);
	~D3D12IndependentUnorderedAccessView() override;

	void CreateView(size_t index, ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	size_t Count_get() const override;
	D3D12_CPU_DESCRIPTOR_HANDLE HandleBase_get() const override;
};