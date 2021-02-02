// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

#include "Shaders/Compiled/VertexShader.hlsl.h"
#include "Shaders/Compiled/PixelShader.hlsl.h"

using namespace std;

D3D12Shader::D3D12Shader() : Super()
	, shaderTypeHash(0)
{
	SetVertexShader(pVertexShader);
	SetPixelShader(pPixelShader);
}

D3D12Shader::~D3D12Shader()
{

}

size_t D3D12Shader::GetHashCode() const
{
	return GetShaderTypeHashCode();
}

size_t D3D12Shader::GetShaderTypeHashCode() const
{
	return shaderTypeHash;
}

void D3D12Shader::SetVertexShader(span<const uint8> shaderBytecode)
{
	vertexShaderBytecode.assign(shaderBytecode.begin(), shaderBytecode.end());
}

void D3D12Shader::SetPixelShader(span<const uint8> shaderBytecode)
{
	pixelShaderBytecode.assign(shaderBytecode.begin(), shaderBytecode.end());
}

void D3D12Shader::CreateShaderPipeline(TRefPtr<String> name, ID3D12Device* device)
{
	D3D12_ROOT_PARAMETER rootParameters[]
	{
		GetRootCBVParameter(0, D3D12_SHADER_VISIBILITY_VERTEX)
	};

	D3D12_ROOT_SIGNATURE_DESC sigDesc = { };
	sigDesc.NumParameters = ARRAYSIZE(rootParameters);
	sigDesc.pParameters = rootParameters;
	sigDesc.NumStaticSamplers = 0;
	sigDesc.pStaticSamplers = nullptr;
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> pBlob, pError;
	HR(D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pBlob, &pError));
	HR(device->CreateRootSignature(0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));

	D3D12_INPUT_ELEMENT_DESC inputElements[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = { };
	pipelineDesc.pRootSignature = rootSignature.Get();
	pipelineDesc.VS = { vertexShaderBytecode.data(), (uint32)vertexShaderBytecode.size() };
	pipelineDesc.PS = { pixelShaderBytecode.data(), (uint32)pixelShaderBytecode.size() };
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	pipelineDesc.SampleMask = 0xFFFFFFFF;
	pipelineDesc.RasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK };
	pipelineDesc.DepthStencilState = { TRUE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS, FALSE, 0, 0 };
	pipelineDesc.InputLayout = { inputElements, ARRAYSIZE(inputElements) };
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_B8G8R8A8_UNORM;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pipelineDesc.SampleDesc = { 1, 0 };
	HR(device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	shaderTypeHash = name->GetHashCode();
}

ID3D12RootSignature* D3D12Shader::RootSignature_get() const
{
	return rootSignature.Get();
}

ID3D12PipelineState* D3D12Shader::PipelineState_get() const
{
	return pipelineState.Get();
}

D3D12_ROOT_PARAMETER D3D12Shader::GetRootCBVParameter(uint32 shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility)
{
	D3D12_ROOT_PARAMETER param = { };
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	param.ShaderVisibility = shaderVisibility;
	param.Constants.ShaderRegister = shaderRegister;
	return param;
}