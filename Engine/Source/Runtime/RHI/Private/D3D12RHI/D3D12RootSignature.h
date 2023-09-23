// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "RHI/RHIRootSignature.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12RootSignature : public NRHIRootSignature
{
	ComPtr<ID3D12RootSignature> pRS;

public:
	ND3D12RootSignature(ID3D12Device& InDevice);

	inline ID3D12RootSignature* Get() const { return pRS.Get(); }
};

#endif