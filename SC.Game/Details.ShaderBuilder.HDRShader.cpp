using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/HDRVertexShader.h"
#include "CompiledShaders/HDRPixelShader.h"

void ShaderBuilder::LoadHDRShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE hdrSource0[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 3, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND },
	};

	D3D12_DESCRIPTOR_RANGE shadowDepth[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 4, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	D3D12_DESCRIPTOR_RANGE reflectionBuffer[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 3, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.PixelShaderRootAccess = true;

	rootSignatureDesc.AddDescriptorTable( hdrSource0 );
	rootSignatureDesc.AddConstantBufferView( 0 );
	rootSignatureDesc.AddConstantBufferView( 1 );
	rootSignatureDesc.AddDescriptorTable( reflectionBuffer );
	rootSignatureDesc.AddDescriptorTable( shadowDepth );

	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER );
	rootSignatureDesc.AddStaticComparisonSampler( D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_BORDER, 1 );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	pipelineDesc.SetVertexShader( pHDRVertexShader, ARRAYSIZE( pHDRVertexShader ) );
	pipelineDesc.SetPixelShader( pHDRPixelShader, ARRAYSIZE( pHDRPixelShader ) );
	pipelineDesc.SetBlendState( 0, true, D3D12_BLEND_INV_DEST_ALPHA, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD );
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	shader.RootParameterIndex =
	{
		{ "GeometryBuffer", 0 },
		{ "Camera", 1 },
		{ "Light", 2 },
		{ "Reflection", 3 },
		{ "ShadowDepth", 4 }
	};

	hdrShader = shader;
}