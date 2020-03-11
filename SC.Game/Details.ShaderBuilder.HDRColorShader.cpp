using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/HDRColorVertexShader.h"
#include "CompiledShaders/HDRColorPixelShader.h"

void ShaderBuilder::LoadHDRColorShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE hdrSource0[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 2, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.PixelShaderRootAccess = true;
	rootSignatureDesc.AddDescriptorTable( hdrSource0 );
	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	pipelineDesc.SetVertexShader( pHDRColorVertexShader, ARRAYSIZE( pHDRColorVertexShader ) );
	pipelineDesc.SetPixelShader( pHDRColorPixelShader, ARRAYSIZE( pHDRColorPixelShader ) );
	pipelineDesc.SetBlendState( 0, true, D3D12_BLEND_DEST_COLOR, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD, D3D12_BLEND_ZERO, D3D12_BLEND_ONE );
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "GeometryBuffer", 0 }
	};

	hdrColorShader = shader;
}