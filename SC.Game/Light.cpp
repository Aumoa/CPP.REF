using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

Light::Light()
{
	lightBuffer[0] = GlobalVar.device->CreateDynamicBuffer( sizeof( LightConstants ) );
	lightBuffer[1] = GlobalVar.device->CreateDynamicBuffer( sizeof( LightConstants ) );

	LightColor = Drawing::Color::White;
	Diffuse = 1.0;
}

void Light::SetGraphicsRootShaderResources( RefPtr<CDeviceContext>& deviceContext )
{
	auto pCommandList = deviceContext->pCommandList.Get();
	int frameIndex = GlobalVar.frameIndex;

	if ( auto slot = deviceContext->Slot["Light"]; slot != -1 )
	{
		memcpy( lightBuffer[frameIndex]->pBlock, &frameResource, sizeof( frameResource ) );
		pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, lightBuffer[frameIndex]->VirtualAddress );
	}

	if ( shadowCast )
	{
		if ( auto slot = deviceContext->Slot["ShadowDepth"]; slot != -1 )
		{
			deviceContext->SetGraphicsRootShaderResources( slot, pShaderResourceView );
		}
	}
}

void Light::SetDepthBuffer( void* pResource )
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srv{ };
	srv.Format = DXGI_FORMAT_R32_FLOAT;
	srv.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srv.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srv.Texture2D.MipLevels = 1;

	pShaderResourceView = GlobalVar.device->CreateShaderResourceView( ( ID3D12Resource* )pResource, &srv );
}

Light::~Light()
{
	GC.Add( lightBuffer );
	GC.Add( pShaderResourceView );
}

Drawing::Color Light::LightColor_get()
{
	return lightColor;
}

void Light::LightColor_set( Drawing::Color value )
{
	lightColor = value;
	frameResource.Color = { lightColor.R, lightColor.G, lightColor.B };
}

double Light::Ambient_get()
{
	return ( double )frameResource.Ambient;
}

void Light::Ambient_set( double value )
{
	frameResource.Ambient = ( float )value;
}

double Light::Diffuse_get()
{
	return ( double )frameResource.Diffuse;
}

void Light::Diffuse_set( double value )
{
	frameResource.Diffuse = ( float )value;
}

double Light::Specular_get()
{
	return ( double )frameResource.Specular;
}

void Light::Specular_set( double value )
{
	frameResource.Specular = ( float )value;
}

bool Light::IsShadowCast_get()
{
	return shadowCast;
}

void Light::IsShadowCast_set( bool value )
{
	shadowCast = value;
	ReadyBuffer();
}