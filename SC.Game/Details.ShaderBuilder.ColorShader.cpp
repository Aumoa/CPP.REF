using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/ColorVertexShader.h"
#include "CompiledShaders/ColorPixelShader.h"

void ShaderBuilder::LoadColorShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE textures[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.InputAssemblerInputLayout = true;
	rootSignatureDesc.VertexShaderRootAccess = true;
	rootSignatureDesc.PixelShaderRootAccess = true;

	rootSignatureDesc.AddConstantBufferView( 0, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddConstantBufferView( 1, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddShaderResourceView( 0, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddDescriptorTable( textures, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddConstantBufferView( 3, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddRoot32BitConstants( 2, 1, D3D12_SHADER_VISIBILITY_VERTEX );

	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 8 );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 3;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R11G11B10_FLOAT;
	pipelineDesc.RTVFormats[2] = DXGI_FORMAT_R16_UINT;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.SetVertexShader( pColorVertexShader, ARRAYSIZE( pColorVertexShader ) );
	pipelineDesc.SetPixelShader( pColorPixelShader, ARRAYSIZE( pColorPixelShader ) );
	pipelineDesc.AddInputLayout( SkinnedVertex::InputElements );
	pipelineDesc.SetDepthStencilState( true );
	pipelineDesc.CullMode = D3D12_CULL_MODE_NONE;
	pipelineDesc.SetBlendState( 0, true, D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD );
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "Camera", 0 },
		{ "World", 1 },
		{ "BoneTransform", 2 },
		{ "Textures", 3 },
		{ "Material", 4 },
		{ "IsSkinned", 5 }
	};

	colorShader = shader;
}