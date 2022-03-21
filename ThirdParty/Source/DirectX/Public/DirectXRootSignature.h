// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXRootSignature : public SDirectXDeviceChild, implements IRHIRootSignature
{
	GENERATED_BODY(SDirectXRootSignature)

public:
	ComPtr<ID3D12RootSignature> pRootSignature;

public:
	SDirectXRootSignature(IRHIDevice* Owner, ComPtr<ID3D12RootSignature> pRootSignature);

	using Super::Dispose;
};