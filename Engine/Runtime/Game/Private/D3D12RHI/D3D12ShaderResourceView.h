// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIShaderResourceView.h"

#include "D3D12Minimal.h"
#include "D3D12OnlineDescriptorIndex.h"

struct D3D12OnlineDescriptorIndex;

class D3D12ShaderResourceView : virtual public Object, virtual public IRHIShaderResourceView
{
public:
	using Super = Object;
	using This = D3D12ShaderResourceView;

private:
	D3D12OnlineDescriptorIndex handle;

public:
	D3D12ShaderResourceView(ID3D12Resource* resource, const D3D12OnlineDescriptorIndex& handle);
	~D3D12ShaderResourceView() override;

	vs_property_get(D3D12_GPU_DESCRIPTOR_HANDLE, Handle);
	D3D12_GPU_DESCRIPTOR_HANDLE Handle_get() const;
};