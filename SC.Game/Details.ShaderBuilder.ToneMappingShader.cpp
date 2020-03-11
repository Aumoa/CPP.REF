using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/ToneMappingVertexShader.h"
#include "CompiledShaders/ToneMappingPixelShader.h"

void ShaderBuilder::LoadToneMappingShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE hdrSource[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.PixelShaderRootAccess = true;
	rootSignatureDesc.AddDescriptorTable( hdrSource );
	rootSignatureDesc.AddConstantBufferView( 0 );
	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_BORDER );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.SetVertexShader( pToneMappingVertexShader, ARRAYSIZE( pToneMappingVertexShader ) );
	pipelineDesc.SetPixelShader( pToneMappingPixelShader, ARRAYSIZE( pToneMappingPixelShader ) );
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "HDRBuffer", 0 },
		{ "HDRConstants", 1 }
	};

	toneMappingShader = shader;
}