// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "RHI/RHIGameShader.h"
#include "D3D12RHI/D3D12Common.h"

class ND3D12GameShader : public NRHIGameShader
{
	ComPtr<ID3D12RootSignature> pRS;
	ComPtr<ID3D12StateObject> pStateObject;
	ComPtr<ID3D12StateObjectProperties> pProperties;

public:
	ND3D12GameShader(ID3D12Device5& InDevice);

	ID3D12RootSignature* GetRootSignature() const noexcept { return pRS.Get(); }
	ID3D12StateObject* GetStateObject() const noexcept { return pStateObject.Get(); }
	ID3D12StateObjectProperties* GetProperties() const noexcept { return pProperties.Get(); }
};

#endif