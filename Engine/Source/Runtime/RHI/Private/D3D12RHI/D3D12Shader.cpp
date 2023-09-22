// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Shader.h"

#if PLATFORM_WINDOWS

ND3D12Shader::ND3D12Shader(ComPtr<ID3D12RootSignature> pInRootSignature, ComPtr<ID3D12PipelineState> pInPipelineState)
	: pRootSignature(std::move(pInRootSignature))
	, pPipelineState(std::move(pInPipelineState))
{
}

#endif