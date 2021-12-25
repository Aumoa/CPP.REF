// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

GENERATE_BODY(SD3D12Shader);

SD3D12Shader::SD3D12Shader(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12RootSignature> rootSignature, ComPtr<ID3D12PipelineState> pipelineState) : Super(InFactory, InDevice, nullptr)
	, _rootSignature(std::move(rootSignature))
	, _pipelineState(std::move(pipelineState))
{
}

void SD3D12Shader::Dispose(bool bDisposing)
{
	_rootSignature.Reset();
	_pipelineState.Reset();

	Super::Dispose(bDisposing);
}