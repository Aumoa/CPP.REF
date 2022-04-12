// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "DirectXRootSignature.h"

GENERATE_BODY(SDirectXRootSignature);

SDirectXRootSignature::SDirectXRootSignature(IRHIDevice* Owner, ComPtr<ID3D12RootSignature> pRootSignature)
	: Super(Owner)
	, pRootSignature(std::move(pRootSignature))
{
}