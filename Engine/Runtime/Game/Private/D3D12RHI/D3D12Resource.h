// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"

#include "D3D12Minimal.h"
#include "RHI/RHIResourceDesc.h"

class D3D12Resource : virtual public Object, virtual public IRHIResource
{
public:
	using Super = Object;
	using This = D3D12Resource;

private:
	ComPtr<ID3D12Resource> resource;
	void* mappingAddr;
	RHIResourceDesc desc;

public:
	D3D12Resource(ID3D12Resource* resource);
	~D3D12Resource() override;

	virtual uint64 GetVirtualAddress() const;
	virtual void* GetMappingAddress() const;
	virtual RHIResourceDesc GetDesc() const;

	void BindMappingAddress();

	vs_property_get(ID3D12Resource*, Resource);
	ID3D12Resource* Resource_get() const;
};