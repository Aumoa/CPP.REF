// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIResource.h"
#include "RHI/RHIStructures.h"

class SD3D12CommandList;

class SD3D12Resource : public SD3D12DeviceChild, implements IRHIResource
{
	GENERATED_BODY(SD3D12Resource)

private:
	ComPtr<ID3D12Resource> _resource;
	ComPtr<ID3D12Resource> _uploadBuf;

public:
	SD3D12Resource(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12Resource> resource, ComPtr<ID3D12Resource> uploadBuf);

	virtual uint64 GetGPUVirtualAddress() override;

	virtual void UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData) = 0;

public:
	DECLARE_GETTER(ID3D12Resource, _resource);
	ID3D12Resource* GetUploadBuf() { return _uploadBuf.Get(); }
};