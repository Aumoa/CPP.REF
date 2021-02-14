// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShaderResourceView.h"

#include "D3D12Minimal.h"
#include "D3D12OfflineDescriptorIndex.h"

struct D3D12OfflineDescriptorIndex;

class D3D12ShaderResourceView : virtual public Object, virtual public IRHIShaderResourceView
{
public:
	using Super = Object;
	using This = D3D12ShaderResourceView;

public:
	D3D12ShaderResourceView();
	~D3D12ShaderResourceView() override;

	vs_property_get(D3D12_CPU_DESCRIPTOR_HANDLE, HandleBase);
	virtual D3D12_CPU_DESCRIPTOR_HANDLE HandleBase_get() const = 0;
};

class D3D12SingleShaderResourceViewNode : public D3D12ShaderResourceView
{
public:
	using Super = D3D12ShaderResourceView;

private:
	D3D12OfflineDescriptorIndex handle;

public:
	D3D12SingleShaderResourceViewNode(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle);
	~D3D12SingleShaderResourceViewNode() override;

	size_t Count_get() const override;
	D3D12_CPU_DESCRIPTOR_HANDLE HandleBase_get() const;
};

class D3D12IndependentShaderResourceView : public D3D12ShaderResourceView, virtual public IRHIShaderResourceView
{
public:
	using Super = D3D12ShaderResourceView;

private:
	ID3D12Device* const device;
	size_t const numSRVs;
	ComPtr<ID3D12DescriptorHeap> heap;
	CD3DX12_CPU_DESCRIPTOR_HANDLE handleBase;
	uint32 increment;

public:
	D3D12IndependentShaderResourceView(ID3D12Device* device, size_t numSRVs);
	~D3D12IndependentShaderResourceView() override;

	void CreateView(size_t index, ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* srvDesc);

	size_t Count_get() const override;
	D3D12_CPU_DESCRIPTOR_HANDLE HandleBase_get() const override;
};