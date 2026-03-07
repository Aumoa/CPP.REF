// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12GeometryRenderPipeline.h"
#include "D3D12Shader.h"

namespace Ayla
{
	D3D12GeometryRenderPipeline::D3D12GeometryRenderPipeline(ID3D12Device5* device, std::vector<SharedPtr<Shader>> shaders)
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
		for (const auto& shader : shaders)
		{
			if (auto d3dShader = dynamic_cast<D3D12Shader*>(shader.Get()))
			{
				const auto& elements = d3dShader->GetInputElements();
				inputElements.insert(inputElements.end(), elements.begin(), elements.end());
			}
		}

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElements.data(), static_cast<UINT>(inputElements.size()) };
		psoDesc.pRootSignature = m_RootSignature.Get();
		psoDesc.VS = shaders[0]->GetBytecode();
		psoDesc.PS = shaders[1]->GetBytecode();
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		HR(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState)));
	}
}