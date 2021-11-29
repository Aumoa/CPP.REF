// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

SD3D12Shader::SD3D12Shader(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12PipelineState> pipelineState) : Super(factory, device, nullptr)
	, _rootSignature(std::move(rootSignature))
	, _pipelineState(std::move(pipelineState))
{
}