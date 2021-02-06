// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIDepthStencilView.h"

#include "D3D12Minimal.h"
#include "D3D12OfflineDescriptorIndex.h"

struct D3D12OfflineDescriptorIndex;

class D3D12DepthStencilView : virtual public Object, virtual public IRHIDepthStencilView
{
public:
	using Super = Object;
	using This = D3D12DepthStencilView;

private:
	D3D12OfflineDescriptorIndex handle;

public:
	D3D12DepthStencilView(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle);
	~D3D12DepthStencilView() override;

	vs_property_get(D3D12_CPU_DESCRIPTOR_HANDLE, Handle);
	D3D12_CPU_DESCRIPTOR_HANDLE Handle_get() const;
};