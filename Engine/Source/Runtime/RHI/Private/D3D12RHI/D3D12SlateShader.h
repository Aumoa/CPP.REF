// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS

#include "RHI/RHISlateShader.h"
#include "D3D12RHI/D3D12Common.h"

class ND3D12SlateShader : public NRHISlateShader
{
	ComPtr<ID3D12RootSignature> pRS;
	ComPtr<ID3D12PipelineState> pPS;

public:
	ND3D12SlateShader(ID3D12Device& InDevice);

	static constexpr uint32 GetScreenBindingRootIndex() noexcept { return 0; }
	static constexpr uint32 GetPaintGeometryRootIndex() noexcept { return 1; }
	static constexpr uint32 GetTextureBindingRootIndex() noexcept { return 2; }
	static constexpr uint32 GetRenderParametersRootIndex() noexcept { return 3; }

	ID3D12RootSignature* GetRootSignature() const noexcept { return pRS.Get(); }
	ID3D12PipelineState* GetPipelineState() const noexcept { return pPS.Get(); }
};

#endif