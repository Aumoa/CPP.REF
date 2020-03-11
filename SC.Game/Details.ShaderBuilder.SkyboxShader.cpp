using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/SkyboxVertexShader.h"
#include "CompiledShaders/SkyboxPixelShader.h"

void ShaderBuilder::LoadSkyboxShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE ranges[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.InputAssemblerInputLayout = true;
	rootSignatureDesc.VertexShaderRootAccess = true;
	rootSignatureDesc.PixelShaderRootAccess = true;
	rootSignatureDesc.AddConstantBufferView( 0 );
	rootSignatureDesc.AddDescriptorTable( ranges );
	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, 8 );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.SetVertexShader( pSkyboxVertexShader, ARRAYSIZE( pSkyboxVertexShader ) );
	pipelineDesc.SetPixelShader( pSkyboxPixelShader, ARRAYSIZE( pSkyboxPixelShader ) );
	pipelineDesc.AddInputLayout( Vertex::InputElements );
	pipelineDesc.CullMode = D3D12_CULL_MODE_FRONT;
	pipelineDesc.SetDepthStencilState( true );
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "Camera", 0 },
		{ "Texture", 1 }
	};

	skyboxShader = shader;
}