using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/HDRComputeSamplingShader.h"
#include "CompiledShaders/HDRComputeLumCompressShader.h"
#include "CompiledShaders/HDRComputeAverageLumShader.h"

void ShaderBuilder::LoadHDRComputeShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE hdrBuffer[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	D3D12_DESCRIPTOR_RANGE halfHDRAndConstants[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 2, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.AddDescriptorTable( hdrBuffer );
	rootSignatureDesc.AddDescriptorTable( halfHDRAndConstants );
	rootSignatureDesc.AddRoot32BitConstants( 0, 1 );
	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{ };
	computePipelineStateDesc.pRootSignature = shader.RootSignature.Get();

	shader.RootParameterIndex =
	{
		{ "HDRBuffer", 0 },
		{ "HDRComputedBuffer", 1 },
		{ "Timer", 2 },
	};
	shader.IsCompute = true;

	computePipelineStateDesc.CS = { pHDRComputeSamplingShader, ARRAYSIZE( pHDRComputeSamplingShader ) };
	HR( pDevice->CreateComputePipelineState( &computePipelineStateDesc, IID_PPV_ARGS( &shader.PipelineState ) ) );
	hdrComputeShader[0] = shader;

	shader.RootSignature = nullptr;
	shader.RootParameterIndex = { };

	computePipelineStateDesc.CS = { pHDRComputeLumCompressShader, ARRAYSIZE( pHDRComputeLumCompressShader ) };
	HR( pDevice->CreateComputePipelineState( &computePipelineStateDesc, IID_PPV_ARGS( &shader.PipelineState ) ) );
	hdrComputeShader[1] = shader;

	computePipelineStateDesc.CS = { pHDRComputeAverageLumShader, ARRAYSIZE( pHDRComputeAverageLumShader ) };
	HR( pDevice->CreateComputePipelineState( &computePipelineStateDesc, IID_PPV_ARGS( &shader.PipelineState ) ) );
	hdrComputeShader[2] = shader;
}