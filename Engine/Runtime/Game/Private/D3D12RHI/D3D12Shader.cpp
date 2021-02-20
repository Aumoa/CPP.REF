// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

#include "Logging/LogMacros.h"

using namespace std;

D3D12Shader::D3D12Shader(ID3D12RootSignature* inRootSignature, ID3D12PipelineState* inPipelineState) : Super()
	, bRaytracing(false)
	, shaderTypeHash(0)
{
	rootSignature = inRootSignature;
	pipelineState = inPipelineState;
}

D3D12Shader::D3D12Shader(ID3D12RootSignature* inRootSignature, ID3D12StateObject* inPipelineState, ID3D12StateObjectProperties* raytracingProperties) : Super()
	, bRaytracing(true)
	, shaderTypeHash(0)
{
	rootSignature = inRootSignature;
	pipelineState = inPipelineState;
}

D3D12Shader::~D3D12Shader()
{

}

void D3D12Shader::InputShader(ID3D12GraphicsCommandList4* inCommandList)
{
	inCommandList->SetGraphicsRootSignature(rootSignature.Get());
	inCommandList->SetPipelineState((ID3D12PipelineState*)pipelineState.Get());
}