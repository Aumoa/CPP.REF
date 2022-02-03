// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXCommandAllocator.h"

GENERATE_BODY(SDirectXCommandAllocator);

SDirectXCommandAllocator::SDirectXCommandAllocator(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator)
	: Super(Owner)
	, pAllocator(std::move(pAllocator))
{
}

void SDirectXCommandAllocator::Reset()
{
	HR(pAllocator->Reset());
}

void SDirectXCommandAllocator::Dispose(bool bDisposing)
{
	pAllocator.Reset();
	Super::Dispose(bDisposing);
}