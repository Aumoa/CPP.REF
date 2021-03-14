// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIRenderTargetView.h"

#include "D3D12Minimal.h"
#include "D3D12OfflineDescriptorIndex.h"

struct D3D12OfflineDescriptorIndex;

class D3D12RenderTargetView : virtual public Object, virtual public IRHIRenderTargetView
{
public:
	using Super = Object;
	using This = D3D12RenderTargetView;

private:
	D3D12OfflineDescriptorIndex handle;

public:
	D3D12RenderTargetView(ID3D12Resource* resource, const D3D12OfflineDescriptorIndex& handle);
	~D3D12RenderTargetView() override;

	vs_property_get(D3D12_CPU_DESCRIPTOR_HANDLE, Handle);
};