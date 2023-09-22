// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "RHI/RHIShader.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12Common.h"

class ND3D12Shader : public NRHIShader
{
	ComPtr<ID3D12RootSignature> pRootSignature;
	ComPtr<ID3D12PipelineState> pPipelineState;

public:
	ND3D12Shader(ComPtr<ID3D12RootSignature> pInRootSignature, ComPtr<ID3D12PipelineState> pInPipelineState);

	inline ID3D12PipelineState* GetPipelineState() const { return pPipelineState.Get(); }
};

#endif