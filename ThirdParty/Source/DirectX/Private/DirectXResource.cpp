// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXResource.h"

using namespace ::libty;

SDirectXResource::SDirectXResource(IRHIDevice* Owner, ComPtr<ID3D12Resource> pResource)
	: Super(Owner)
	, pResource(std::move(pResource))
{
}

RHIResourceDesc SDirectXResource::GetDesc()
{
	D3D12_RESOURCE_DESC desc = pResource->GetDesc();
	return reinterpret_cast<const RHIResourceDesc&>(desc);
}

void SDirectXResource::Dispose(bool bDisposing)
{
	pResource.Reset();
	Super::Dispose(bDisposing);
}