// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIResource.h"
#include "RHI/DirectXCommon.h"

class SD3D12Resource : public SD3D12DeviceChild, implements IRHIResource
{
	GENERATED_BODY(SD3D12Resource)

private:
	ComPtr<ID3D12Resource> _resource;

public:
	SD3D12Resource(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12Resource> resource);

	virtual uint64 GetGPUVirtualAddress() override;

public:
	DECLARE_GETTER(ID3D12Resource, _resource);
};