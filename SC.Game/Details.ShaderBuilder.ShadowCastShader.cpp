using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/ShadowCastVertexShader.h"
#include "CompiledShaders/ShadowCastPixelShader.h"

void ShaderBuilder::LoadShadowCastShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE diffuseMap0[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.InputAssemblerInputLayout = true;
	rootSignatureDesc.VertexShaderRootAccess = true;
	rootSignatureDesc.PixelShaderRootAccess = true;

	rootSignatureDesc.AddConstantBufferView( 0, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddConstantBufferView( 1, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddDescriptorTable( diffuseMap0, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddConstantBufferView( 3, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddShaderResourceView( 0, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddRoot32BitConstants( 2, 1, D3D12_SHADER_VISIBILITY_VERTEX );

	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 8 );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.SetVertexShader( pShadowCastVertexShader, ARRAYSIZE( pShadowCastVertexShader ) );
	pipelineDesc.SetPixelShader( pShadowCastPixelShader, ARRAYSIZE( pShadowCastPixelShader ) );
	pipelineDesc.AddInputLayout( SkinnedVertex::InputElements );
	pipelineDesc.SetDepthStencilState( true );
	pipelineDesc.CullMode = D3D12_CULL_MODE_NONE;
	pipelineDesc.SlopeScaledDepthBias = 3.0;
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "Light", 0 },
		{ "World", 1 },
		{ "Textures", 2 },
		{ "Material", 3 },
		{ "BoneTransform", 4 },
		{ "IsSkinned", 5 }
	};

	shadowCastShader = shader;
}