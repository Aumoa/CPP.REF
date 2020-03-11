#include "Details.ShaderBuilder.h"
using namespace SC;
using namespace SC::Game::Details;

ShaderModule ShaderBuilder::colorShader;
ShaderModule ShaderBuilder::skyboxShader;
ShaderModule ShaderBuilder::hdrShader;
ShaderModule ShaderBuilder::hdrColorShader;
ShaderModule ShaderBuilder::toneMappingShader;
ShaderModule ShaderBuilder::shadowCastShader;
ShaderModule ShaderBuilder::hdrComputeShader[3];

ShaderModule ShaderBuilder::integratedUIShader;
ShaderModule ShaderBuilder::textAndRectShader;

void ShaderModule::SetRootSignatureAndPipelineState( RefPtr<CDeviceContext>& deviceContext )
{
	auto pCommandList = deviceContext->pCommandList.Get();

	if ( RootSignature )
		if ( IsCompute )
			pCommandList->SetComputeRootSignature( RootSignature.Get() );
		else
			pCommandList->SetGraphicsRootSignature( RootSignature.Get() );

	if ( PipelineState )
		pCommandList->SetPipelineState( PipelineState.Get() );
}

void ShaderModule::SetRootParameterIndex( RefPtr<CDeviceContext>& deviceContext )
{
	if ( RootSignature )
	{
		deviceContext->SetSlotMap( RootParameterIndex );
	}
}

void ShaderModule::SetAll( RefPtr<CDeviceContext>& deviceContext )
{
	SetRootSignatureAndPipelineState( deviceContext );
	SetRootParameterIndex( deviceContext );
}

void ShaderBuilder::Initialize()
{
	LoadColorShader();
	LoadSkyboxShader();
	LoadHDRShader();
	LoadHDRColorShader();
	LoadToneMappingShader();
	LoadShadowCastShader();
	LoadHDRComputeShader();

	LoadIntegratedUIShader();
	LoadTextAndRectShader();
}

ShaderModule ShaderBuilder::ColorShader_get()
{
	return colorShader;
}

ShaderModule ShaderBuilder::SkyboxShader_get()
{
	return skyboxShader;
}

ShaderModule ShaderBuilder::HDRShader_get()
{
	return hdrShader;
}

ShaderModule ShaderBuilder::HDRColorShader_get()
{
	return hdrColorShader;
}

ShaderModule ShaderBuilder::ToneMappingShader_get()
{
	return toneMappingShader;
}

ShaderModule ShaderBuilder::ShadowCastShader_get()
{
	return shadowCastShader;
}

ShaderModule ShaderBuilder::HDRComputeShader_get( int param0 )
{
	return hdrComputeShader[param0];
}

ShaderModule ShaderBuilder::IntegratedUIShader_get()
{
	return integratedUIShader;
}

ShaderModule ShaderBuilder::TextAndRectShader_get()
{
	return textAndRectShader;
}