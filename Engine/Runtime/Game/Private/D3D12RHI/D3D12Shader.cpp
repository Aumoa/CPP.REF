// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

#include "Logging/LogMacros.h"

using namespace std;

D3D12Shader::D3D12Shader(ID3D12RootSignature* inRootSignature, ID3D12PipelineState* inPipelineState) : Super()
	, shaderTypeHash(0)
{
	rootSignature = inRootSignature;
	pipelineState = inPipelineState;
}

D3D12Shader::~D3D12Shader()
{

}

ID3D12RootSignature* D3D12Shader::RootSignature_get() const
{
	return rootSignature.Get();
}

ID3D12PipelineState* D3D12Shader::PipelineState_get() const
{
	return pipelineState.Get();
}