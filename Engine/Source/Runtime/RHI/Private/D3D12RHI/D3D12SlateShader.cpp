// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12SlateShader.h"

#if PLATFORM_WINDOWS

#include "D3D12RHI/D3D12SlateShader.h"
#include "SlateElementVertexShader.fx.h"
#include "SlateElementPixelShader.fx.h"

ND3D12SlateShader::ND3D12SlateShader(ID3D12Device& InDevice)
{
	D3D12_DESCRIPTOR_RANGE RS1Ranges[] =
	{
		{
			.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
			.NumDescriptors = 1,
			.BaseShaderRegister = 0,
			.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND,
		}
	};

	D3D12_ROOT_PARAMETER RSParameters[] =
	{
		{
			// ScreenInformation for VS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,
			.Constants =
			{
				.Num32BitValues = 2
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
		},
		{
			// PaintGeometry for VS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
			.Descriptor =
			{
				.ShaderRegister = 1
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
		},
		{
			// Texture for PS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE,
			.DescriptorTable =
			{
				.NumDescriptorRanges = (UINT)AE_ARRAYSIZE(RS1Ranges),
				.pDescriptorRanges = RS1Ranges
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		},
		{
			// RenderParams for PS
			.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
			.Descriptor =
			{
				.ShaderRegister = 0
			},
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		},
	};

	D3D12_STATIC_SAMPLER_DESC RSStaticParameters[] =
	{
		{
			.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER,
			.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL
		}
	};

	D3D12_ROOT_SIGNATURE_DESC RSDesc =
	{
		.NumParameters = (UINT)AE_ARRAYSIZE(RSParameters),
		.pParameters = RSParameters,
		.NumStaticSamplers = 1,
		.pStaticSamplers = RSStaticParameters,
		.Flags = D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
	};

	ComPtr<ID3DBlob> pSerializedBlob, pError;
	if (HRESULT Hr = D3D12SerializeRootSignature(&RSDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &pSerializedBlob, &pError); FAILED(Hr))
	{
		if (pError)
		{
			PlatformProcess::OutputDebugString(String::Format(TEXT("{}\n"), String::FromLiteral((const char*)pError->GetBufferPointer())));
		}
		HR(Hr);
	}

	HR(InDevice.CreateRootSignature(0, pSerializedBlob->GetBufferPointer(), pSerializedBlob->GetBufferSize(), IID_PPV_ARGS(&pRS)));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc =
	{
		.pRootSignature = pRS.Get(),
		.VS = { SlateElementVertexShader, AE_ARRAYSIZE(SlateElementVertexShader) },
		.PS = { SlateElementPixelShader, AE_ARRAYSIZE(SlateElementPixelShader) },
		.BlendState =
		{
			.AlphaToCoverageEnable = FALSE,
			.IndependentBlendEnable = FALSE,
			.RenderTarget =
			{
				{
					.BlendEnable = TRUE,
					.SrcBlend = D3D12_BLEND_SRC_ALPHA,
					.DestBlend = D3D12_BLEND_INV_SRC_ALPHA,
					.BlendOp = D3D12_BLEND_OP_ADD,
					.SrcBlendAlpha = D3D12_BLEND_ONE,
					.DestBlendAlpha = D3D12_BLEND_ZERO,
					.BlendOpAlpha = D3D12_BLEND_OP_ADD,
					.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL
				}
			}
		},
		.SampleMask = 0xFFFFFFFF,
		.RasterizerState =
		{
			.FillMode = D3D12_FILL_MODE_SOLID,
			.CullMode = D3D12_CULL_MODE_BACK,
		},
		.DepthStencilState =
		{
			.DepthEnable = FALSE,
			.StencilEnable = FALSE
		},
		.InputLayout =
		{
			.pInputElementDescs = NULL,
			.NumElements = 0
		},
		.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
		.NumRenderTargets = 1,
		.RTVFormats = { DXGI_FORMAT_R8G8B8A8_UNORM },
		.SampleDesc = { 1, 0 }
	};

	HR(InDevice.CreateGraphicsPipelineState(&PSDesc, IID_PPV_ARGS(&pPS)));
}

#endif