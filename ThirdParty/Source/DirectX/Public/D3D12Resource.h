// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIResource.h"
#include "RHI/RHIStructures.h"

class SD3D12CommandList;

class DIRECTX_API SD3D12Resource : public SD3D12DeviceChild, implements IRHIResource
{
	GENERATED_BODY(SD3D12Resource)

private:
	ID3D12Resource* Resource = nullptr;
	ID3D12Resource* UploadBuf = nullptr;

public:
	SD3D12Resource(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID3D12Resource* Resource, ID3D12Resource* UploadBuf);

	using Super::Dispose;
	virtual uint64 GetGPUVirtualAddress() override;

	virtual void UpdateSubresource(SD3D12CommandList* commandList, uint32 subresource, const RHISubresourceData* uploadData) = 0;

protected:
	virtual void Dispose(bool bDisposing) override;

public:
	DECLARE_GETTER(ID3D12Resource, Resource);
	ID3D12Resource* GetUploadBuf() { return UploadBuf; }
};