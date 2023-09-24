// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12GraphicsPipelineState.h"

#if PLATFORM_WINDOWS

#include "SlateElementVertexShader.fx.h"
#include "SlateElementPixelShader.fx.h"

ND3D12GraphicsPipelineState::ND3D12GraphicsPipelineState(ID3D12Device& InDevice, ID3D12RootSignature& RS)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSDesc =
	{
		.pRootSignature = &RS,
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