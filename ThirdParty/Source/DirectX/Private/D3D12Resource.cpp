// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Resource.h"

GENERATE_BODY(SD3D12Resource);

SD3D12Resource::SD3D12Resource(SDXGIFactory* InFactory, SD3D12Device* InDevice, ID3D12Resource* Resource, ID3D12Resource* UploadBuf)
	: Super(InFactory, InDevice, Resource)
	, Resource(Resource)
	, UploadBuf(UploadBuf)
{
}

uint64 SD3D12Resource::GetGPUVirtualAddress()
{
	return Resource->GetGPUVirtualAddress();
}

void SD3D12Resource::Dispose(bool bDisposing)
{
	Resource = nullptr;
	UploadBuf = nullptr;

	Super::Dispose(bDisposing);
}