// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"

#include "D3D12Minimal.h"

class D3D12Resource : virtual public Object, virtual public IRHIResource
{
public:
	using Super = Object;
	using This = D3D12Resource;

private:
	ComPtr<ID3D12Resource> resource;

public:
	D3D12Resource(ID3D12Resource* resource);
	~D3D12Resource() override;

	vs_property_get(ID3D12Resource*, Resource);
	ID3D12Resource* Resource_get() const;
};