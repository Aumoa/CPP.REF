// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12GeometryRenderPipeline.h"
#include "D3D12Shader.h"
#include "D3D12CommandBuffer.h"
#include "D3D12Buffer.h"
#include "Rendering/VertexFactory.h"
#include "Rendering/ShaderType.h"

namespace Ayla
{
	D3D12GeometryRenderPipeline::D3D12GeometryRenderPipeline(ID3D12Device5* device, SharedPtr<Shader> shader)
	{
		D3D12_ROOT_PARAMETER rootParameters[1] =
		{
			{
				.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV,
				.Constants =
				{
					.ShaderRegister = 0
				},
				.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX
			}
		};

		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc =
		{
			.NumParameters = static_cast<UINT>(AE_ARRAYSIZE(rootParameters)),
			.pParameters = rootParameters,
			.NumStaticSamplers = 0,
			.pStaticSamplers = nullptr,
			.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
		};

		ComPtr<ID3DBlob> serializedRootSig = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		HR(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1,
			serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf()));

		HR(device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
			serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
		if (auto* vf = shader->GetVertexFactory())
		{
			const auto& elements = vf->GetInputElements();
			inputElements.reserve(elements.size());
			
			for (const auto& element : elements)
			{
				D3D12_INPUT_ELEMENT_DESC d3dElement = {};
				d3dElement.SemanticName = ToSemanticName(element.SemanticName);
				d3dElement.SemanticIndex = static_cast<UINT>(element.SemanticIndex);
				d3dElement.Format = ToDXGIFormat(element.Format);
				d3dElement.InputSlot = static_cast<UINT>(element.InputSlot);
				d3dElement.AlignedByteOffset = static_cast<UINT>(element.AlignedByteOffset);
				d3dElement.InputSlotClass = (element.InstanceDataStepRate > 0) 
					? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA 
					: D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				d3dElement.InstanceDataStepRate = static_cast<UINT>(element.InstanceDataStepRate);
				inputElements.emplace_back(d3dElement);
			}
		}

		const auto& vs = shader->GetBytecode(ShaderType::Vertex);
		const auto& ps = shader->GetBytecode(ShaderType::Pixel);
		const auto& ds = shader->GetBytecode(ShaderType::Domain);
		const auto& hs = shader->GetBytecode(ShaderType::Hull);
		const auto& gs = shader->GetBytecode(ShaderType::Geometry);

		D3D12_RASTERIZER_DESC rasterizerDesc =
		{
			D3D12_FILL_MODE_SOLID,
			D3D12_CULL_MODE_BACK,
			FALSE,
			0,
			0,
			0,
			FALSE,
			FALSE,
			FALSE,
			0,
			D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
		};

		D3D12_BLEND_DESC blendState =
		{
			.AlphaToCoverageEnable = FALSE,
			.IndependentBlendEnable = FALSE,
		};

		for (auto& rt : blendState.RenderTarget)
		{
			rt.BlendEnable = FALSE;
			rt.LogicOpEnable = FALSE;
			rt.SrcBlend = D3D12_BLEND_ONE;
			rt.DestBlend = D3D12_BLEND_ZERO;
			rt.BlendOp = D3D12_BLEND_OP_ADD;
			rt.SrcBlendAlpha = D3D12_BLEND_ONE;
			rt.DestBlendAlpha = D3D12_BLEND_ZERO;
			rt.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			rt.LogicOp = D3D12_LOGIC_OP_NOOP;
			rt.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}

		D3D12_DEPTH_STENCIL_DESC depthStencilDesc =
		{
			.DepthEnable = TRUE,
			.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL,
			.DepthFunc = D3D12_COMPARISON_FUNC_LESS,
			.StencilEnable = FALSE,
			.StencilReadMask = 0xFF,
			.StencilWriteMask = 0xFF,
			.FrontFace = {},
			.BackFace = {}
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElements.data(), static_cast<UINT>(inputElements.size()) };
		psoDesc.pRootSignature = m_RootSignature.Get();
		psoDesc.VS = { vs.Bytecode.data(), vs.Bytecode.size() };
		psoDesc.PS = { ps.Bytecode.data(), ps.Bytecode.size() };
		psoDesc.DS = { ds.Bytecode.data(), ds.Bytecode.size() };
		psoDesc.HS = { hs.Bytecode.data(), hs.Bytecode.size() };
		psoDesc.GS = { gs.Bytecode.data(), gs.Bytecode.size() };
		psoDesc.RasterizerState = rasterizerDesc;
		psoDesc.BlendState = blendState;
		psoDesc.DepthStencilState = depthStencilDesc;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		psoDesc.SampleDesc.Count = 1;
		HR(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineState)));
	}

	void D3D12GeometryRenderPipeline::SetCameraBufferView(CommandBuffer* cmd, Buffer* buffer, size_t offset)
	{
		auto d3dCmd = (D3D12CommandBuffer*)cmd;
		auto d3dBuffer = (D3D12Buffer*)buffer;
		d3dCmd->GetCommandBuffer()->SetGraphicsRootConstantBufferView(0, d3dBuffer->GetResource()->GetGPUVirtualAddress() + offset);
	}
}