// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "ShaderWorkspaces/ColorShaderWorkspace.h"
#include "ShaderWorkspaces/ColorShaderHLSL/VertexShader.hlsl"
#include "ShaderWorkspaces/ColorShaderHLSL/PixelShader.hlsl"

GENERATE_BODY(SColorShaderWorkspace);

SColorShaderWorkspace::SColorShaderWorkspace(IRHIDevice* device)
	: Super(device)
{
	AddVertexShader(L"ColorShaderHLSL/VertexShader.hlsl", "VSMain", ColorShaderHLSL::VertexShader);
	AddPixelShader(L"ColorShaderHLSL/PixelShader.hlsl", "PSMain", ColorShaderHLSL::PixelShader);
}

IRHIRootSignature* SColorShaderWorkspace::CreateRootSignature(IRHIShaderCodeWorkspace* workspace)
{
	RHIRootSignatureDesc rsdesc =
	{
	};

	auto* device = workspace->GetDevice();
	return device->CreateRootSignature(rsdesc);
}

IRHIPipelineState* SColorShaderWorkspace::CreateGraphicsPipelineState(IRHIShaderCodeWorkspace* workspace, IRHIRootSignature* signature)
{
	auto* vs = workspace->GetCompiledShaderCodeBlob("VSMain");
	auto* ps = workspace->GetCompiledShaderCodeBlob("PSMain");

	RHIGraphicsPipelineStateDesc psdesc =
	{
		.pRootSignature = signature,
		.VS =
		{
			.pShaderBytecode = vs->GetBufferPointer(),
			.BytecodeLength = vs->GetBufferSize()
		},
		.PS =
		{
			.pShaderBytecode = ps->GetBufferPointer(),
			.BytecodeLength = ps->GetBufferSize()
		},
		.BlendState =
		{
			.RenderTarget =
			{
				{
					.RenderTargetWriteMask = ERHIColorWriteEnable::All
				}
			}
		},
		.SampleMask = 0xFFFFFFFF,
		.RasterizerState =
		{
			.FillMode = ERHIFillMode::Solid,
			.CullMode = ERHICullMode::None,
		},
		.PrimitiveTopologyType = ERHIPrimitiveTopologyType::Triangle,
		.NumRenderTargets = 1,
		.RTVFormats = { ERHIPixelFormat::B8G8R8A8_UNORM },
		.SampleDesc =
		{
			.Count = 1,
			.Quality = 0
		}
	};

	auto* device = workspace->GetDevice();
	return device->CreateGraphicsPipelineState(psdesc);
}