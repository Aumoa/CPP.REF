// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12Shader.h"

#include "RHI/RHIShaderDescription.h"
#include "Logging/LogMacros.h"

using namespace std;

D3D12Shader::D3D12Shader() : Super()
	, shaderTypeHash(0)
{

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

void D3D12Shader::CreateShaderPipeline(const RHIShaderDescription& inShaderDesc, ID3D12Device* device)
{
	if (!inShaderDesc.IsValid)
	{
		SE_LOG(LogShader, Error, L"Shader description is not valid.");
		return;
	}

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
	pipelineDesc.VS = { inShaderDesc.VS.pShaderBytecode, inShaderDesc.VS.BytecodeLength };
	pipelineDesc.PS = { inShaderDesc.PS.pShaderBytecode, inShaderDesc.PS.BytecodeLength };
	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	pipelineDesc.SampleMask = 0xFFFFFFFF;
	pipelineDesc.RasterizerState = { D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_BACK };
	pipelineDesc.DepthStencilState = { TRUE, D3D12_DEPTH_WRITE_MASK_ALL, D3D12_COMPARISON_FUNC_LESS, FALSE, 0, 0 };
	pipelineDesc.InputLayout = { inputElements, ARRAYSIZE(inputElements) };
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	pipelineDesc.NumRenderTargets = (UINT)inShaderDesc.RTVFormats.size();
	ApplyArray<true>(pipelineDesc.RTVFormats, span(reinterpret_cast<const vector<DXGI_FORMAT>&>(inShaderDesc.RTVFormats)));
	pipelineDesc.DSVFormat = (DXGI_FORMAT)inShaderDesc.DSVFormat;
	pipelineDesc.SampleDesc = { 1, 0 };
	HR(device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState)));

	shaderTypeHash = inShaderDesc.ShaderName->GetHashCode();
}

bool D3D12Shader::IsInitialized_get() const
{
	return bShaderInitialized;
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

template<bool bFastCopy, class T>
inline void D3D12Shader::ApplyArray(T* outArray, std::span<T const> inArray)
{
	if constexpr (bFastCopy)
	{
		memcpy(outArray, inArray.data(), inArray.size_bytes());
	}
	else
	{
		for (size_t i = 0; i < inArray.size(); ++i)
		{
			outArray[i] = inArray[i];
		}
	}
}