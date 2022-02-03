// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXResource.h"

GENERATE_BODY(SDirectXResource);

SDirectXResource::SDirectXResource(IRHIDevice* Owner, ComPtr<ID3D12Resource> pResource)
	: Super(Owner)
	, pResource(std::move(pResource))
{
}

void SDirectXResource::Dispose(bool bDisposing)
{
	pResource.Reset();
	Super::Dispose(bDisposing);
}