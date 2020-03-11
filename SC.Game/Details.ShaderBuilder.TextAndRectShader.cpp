using namespace SC;
using namespace SC::Game::Details;

#include "CompiledShaders/TextAndRectVertexShader.h"
#include "CompiledShaders/TextAndRectPixelShader.h"

void ShaderBuilder::LoadTextAndRectShader()
{
	ShaderModule shader = IntegratedUIShader_get();
	auto pDevice = GlobalVar.device->pDevice.Get();

	CGraphicsPipelineStateDesc pipelineDesc( shader.RootSignature );
	pipelineDesc.RTVCount = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipelineDesc.SetVertexShader( pTextAndRectVertexShader, ARRAYSIZE( pTextAndRectVertexShader ) );
	pipelineDesc.SetPixelShader( pTextAndRectPixelShader, ARRAYSIZE( pTextAndRectPixelShader ) );
	pipelineDesc.SetBlendState( 0, true, D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA );
	HR( pDevice->CreateGraphicsPipelineState( pipelineDesc.Get(), IID_PPV_ARGS( &shader.PipelineState ) ) );

	textAndRectShader = shader;
}