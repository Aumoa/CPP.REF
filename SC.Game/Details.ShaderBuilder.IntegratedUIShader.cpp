using namespace SC;
using namespace SC::Game::Details;

void ShaderBuilder::LoadIntegratedUIShader()
{
	ShaderModule shader;
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_DESCRIPTOR_RANGE textBuffer[]
	{
		{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND }
	};

	CRootSignatureDesc rootSignatureDesc;
	rootSignatureDesc.InputAssemblerInputLayout = true;
	rootSignatureDesc.VertexShaderRootAccess = true;
	rootSignatureDesc.PixelShaderRootAccess = true;

	rootSignatureDesc.AddRoot32BitConstants( 0, 2 );
	rootSignatureDesc.AddRoot32BitConstants( 1, 2 );
	rootSignatureDesc.AddShaderResourceView( 0, D3D12_SHADER_VISIBILITY_VERTEX );
	rootSignatureDesc.AddDescriptorTable( textBuffer, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddConstantBufferView( 2, D3D12_SHADER_VISIBILITY_PIXEL );
	rootSignatureDesc.AddRoot32BitConstants( 3, 2, D3D12_SHADER_VISIBILITY_PIXEL );

	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_BORDER );
	rootSignatureDesc.AddStaticSampler( D3D12_FILTER_MIN_MAG_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER, 0, 1 );

	shader.RootSignature = GlobalVar.device->CreateRootSignature( *rootSignatureDesc.Get() );

	shader.RootParameterIndex =
	{
		{ "ScreenRes", 0 },
		{ "Cursor", 1 },
		{ "ShaderInfo", 2 },
		{ "Texture", 3 },
		{ "Brush", 4 },
		{ "RenderType", 5 }
	};

	integratedUIShader = shader;
}