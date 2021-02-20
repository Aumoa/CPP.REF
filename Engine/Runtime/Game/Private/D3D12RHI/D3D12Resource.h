// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"
#include "ID3D12ResourceBase.h"

#include "D3D12Minimal.h"
#include "RHI/RHIResourceDesc.h"

class D3D12Resource : virtual public Object, virtual public IRHIResource, virtual public ID3D12ResourceBase
{
public:
	using Super = Object;
	using This = D3D12Resource;

private:
	ComPtr<ID3D12Resource> resource;
	void* mappingAddr;
	RHIResourceDesc desc;
	uint64 virtualAddress;

public:
	D3D12Resource(ID3D12Resource* inResource);
	D3D12Resource(ID3D12Resource* inResource, uint64 inVirtualAddress, void* inMappingAddr);
	~D3D12Resource() override;

	virtual uint64 GetVirtualAddress() const;
	virtual void* GetMappingAddress() const;
	virtual RHIResourceDesc GetDesc() const;

	virtual ID3D12Resource* Resource_get() const;

private:
	void BindMappingAddress();
	void UpdateResourceDesc();
};