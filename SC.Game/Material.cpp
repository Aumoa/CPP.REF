using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

ComPtr<ID3D12Resource> Material::pReflectionBuffer;
Material::Reflection* Material::reflectionBufferPtr;
RefPtr<CShaderResourceView> Material::pShaderResourceView;
int Material::capacity;
int Material::reference_count;
vector<bool> Material::locked;
RefPtr<CShaderResourceView> Material::pNullSRV;

void Material::SetGraphicsRootConstantBuffers( RefPtr<CDeviceContext>& deviceContext )
{
	auto pCommandList = deviceContext->pCommandList.Get();

	memcpy( constantBuffer->pBlock, &frameResourceConstants, sizeof( frameResourceConstants ) );
	memcpy( reflectionBufferPtr + lockIndex, &frameResourceReflection, sizeof( frameResourceReflection ) );

	if ( auto slot = deviceContext->Slot["Material"]; slot != -1 )
	{
		pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, constantBuffer->VirtualAddress );
	}

	if ( auto slot = deviceContext->Slot["Textures"]; slot != -1 )
	{
		CShaderResourceView* ppSRVs[3]{ };
		
		if ( diffuseMap && diffuseMap->Lock( deviceContext ) )
		{
			ppSRVs[0] = diffuseMap->pShaderResourceView.Get();
		}
		else ppSRVs[0] = pNullSRV.Get();

		if ( diffuseLayerMap && diffuseLayerMap->Lock( deviceContext ) )
		{
			ppSRVs[1] = diffuseLayerMap->pShaderResourceView.Get();
		}
		else ppSRVs[1] = pNullSRV.Get();

		if ( normalMap && normalMap->Lock( deviceContext ) )
		{
			normalMap->Lock( deviceContext );
			ppSRVs[2] = normalMap->pShaderResourceView.Get();
		}
		else ppSRVs[2] = pNullSRV.Get();

		deviceContext->SetGraphicsRootShaderResources( ( UINT )slot, ARRAYSIZE( ppSRVs ), ppSRVs );
	}
}

void Material::SetGraphicsRootShaderResources( RefPtr<CDeviceContext>& deviceContext )
{
	if ( auto slot = deviceContext->Slot["Reflection"]; slot != -1 )
	{
		deviceContext->SetGraphicsRootShaderResources( slot, pShaderResourceView );
	}
}

Material::Material( String name ) : Assets( name )
{
	lockIndex = Lock( this );

	Ambient = 1;
	Diffuse = 1;
	Specular = 1;
	SpecExp = 32;

	constantBuffer = GlobalVar.device->CreateDynamicBuffer( sizeof( Reflection ), 256 );

	// 참조 횟수를 증가시킵니다.
	++reference_count;
}

Material::~Material()
{
	GC.Add( constantBuffer );

	if ( --reference_count == 0 )
	{
		// 전역 개체를 명시적으로 제거합니다.
		pReflectionBuffer = nullptr;
		pShaderResourceView = nullptr;
		capacity = 0;
		pNullSRV = nullptr;
	}
}

double Material::Ambient_get()
{
	return ( double )frameResourceReflection.Ambient;
}

void Material::Ambient_set( double value )
{
	frameResourceReflection.Ambient = ( float )value;
}

double Material::Diffuse_get()
{
	return ( double )frameResourceReflection.Diffuse;
}

void Material::Diffuse_set( double value )
{
	frameResourceReflection.Diffuse = ( float )value;
}

double Material::Specular_get()
{
	return ( double )frameResourceReflection.Specular;
}

void Material::Specular_set( double value )
{
	frameResourceReflection.Specular = ( float )value;
}

double Material::SpecExp_get()
{
	return ( double )frameResourceReflection.SpecExp;
}

void Material::SpecExp_set( double value )
{
	frameResourceReflection.SpecExp = ( float )value;
}

RefPtr<Texture2D> Material::DiffuseMap_get()
{
	return diffuseMap;
}

void Material::DiffuseMap_set( RefPtr<Texture2D> value )
{
	if ( ( bool )diffuseMap != ( bool )value )
	{
		frameResourceConstants.DiffuseMap += ( bool )value ? 1 : -1;
	}
	diffuseMap = value;
}

RefPtr<Texture2D> Material::DiffuseLayerMap_get()
{
	return diffuseLayerMap;
}

void Material::DiffuseLayerMap_set( RefPtr<Texture2D> value )
{
	if ( ( bool )diffuseLayerMap != ( bool )value )
	{
		frameResourceConstants.DiffuseMap += ( bool )value ? 1 : -1;
	}
	diffuseLayerMap = value;
}

RefPtr<Texture2D> Material::NormalMap_get()
{
	return normalMap;
}

void Material::NormalMap_set( RefPtr<Texture2D> value )
{
	normalMap = value;
	frameResourceConstants.NormalMap = ( int )( bool )value;
}

RenderQueueLayer Material::Layer_get()
{
	return layer;
}

void Material::Layer_set( RenderQueueLayer value )
{
	layer = value;
}

int Material::Lock( Material* ptr )
{
	if ( !pReflectionBuffer )
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Texture2D.MipLevels = 1;

		pNullSRV = GlobalVar.device->CreateShaderResourceView( nullptr, &srvDesc );

		// 버퍼가 없을 경우 초기 버퍼를 생성합니다.
		return Realloc( 256 );
	}

	for ( int i = 0; i < capacity; ++i )
	{
		// 빈 공간을 찾았으면 그곳의 인덱스를 반환합니다.
		if ( !locked[i] )
		{
			locked[i] = true;
			return i;
		}
	}

	// 빈 공간을 찾지 못했으면 전체 크기를 늘립니다.
	// 인덱스는 늘린 공간에서 첫 번째 위치입니다.
	return Realloc( capacity + 256 );
}

int Material::Realloc( int capacity )
{
	auto pDevice = GlobalVar.device->pDevice.Get();

	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };

	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = sizeof( Reflection ) * 256;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pReflectionBuffer ) ) );

	HR( pReflectionBuffer->Map( 0, nullptr, ( void** )&reflectionBufferPtr ) );

	// 버퍼에 대한 셰이더 자원 서술자를 생성합니다.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ };
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Buffer.NumElements = capacity;
	srvDesc.Buffer.StructureByteStride = sizeof( Reflection );
	pShaderResourceView = GlobalVar.device->CreateShaderResourceView( pReflectionBuffer.Get(), &srvDesc );

	// 캐퍼시티를 다시 설정합니다.
	auto ret = Material::capacity;
	locked.resize( capacity, false );
	Material::capacity = capacity;
	return ret;
}