// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXCommon.h"
#include "DirectXResource.h"
#include "DirectXCommandList.h"

using namespace ::libty;

void ::libty::DirectX::ReplaceNativePointer(std::vector<D3D12_RESOURCE_BARRIER>& dst, std::span<const RHIResourceBarrier> src)
{
	dst.resize(src.size());

	for (size_t i = 0; i < src.size(); ++i)
	{
		dst[i] = reinterpret_cast<const D3D12_RESOURCE_BARRIER&>(src[i]);
		switch (src[i].Type)
		{
		case ERHIResourceBarrierType::Transition:
			dst[i].Transition.pResource = Cast<SDirectXResource>(src[i].Transition.pResource)->pResource.Get();
			break;
		case ERHIResourceBarrierType::Aliasing:
			dst[i].Aliasing.pResourceBefore = Cast<SDirectXResource>(src[i].Aliasing.pResourceBefore)->pResource.Get();
			dst[i].Aliasing.pResourceAfter = Cast<SDirectXResource>(src[i].Aliasing.pResourceAfter)->pResource.Get();
			break;
		case ERHIResourceBarrierType::UAV:
			dst[i].UAV.pResource = Cast<SDirectXResource>(src[i].UAV.pResource)->pResource.Get();
			break;
		}
	}
}

void ::libty::DirectX::ReplaceNativePointer(std::vector<ID3D12CommandList*>& dst, std::span<IRHIGraphicsCommandList* const> src)
{
	dst.resize(src.size());

	for (size_t i = 0; i < src.size(); ++i)
	{
		dst[i] = Cast<SDirectXCommandList>(src[i])->pCommandList.Get();
	}
}