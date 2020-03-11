using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

void DirectionalLight::ReadyBuffer()
{
	if ( !pShadowDepthMap )
	{
		auto pDevice = GlobalVar.device->pDevice.Get();

		D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_DEFAULT };

		D3D12_RESOURCE_DESC textureDesc{ };
		textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		textureDesc.Width = 2048;
		textureDesc.Height = 2048;
		textureDesc.DepthOrArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.SampleDesc = { 1, 0 };
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		
		D3D12_CLEAR_VALUE depthClear{ DXGI_FORMAT_D32_FLOAT };
		depthClear.DepthStencil.Depth = 1.0f;

		HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &depthClear, IID_PPV_ARGS( &pShadowDepthMap ) ) );
		SetDepthBuffer( pShadowDepthMap.Get() );

		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = 1;
		HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pDSVHeap ) ) );
		
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{ };
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

		pDevice->CreateDepthStencilView( pShadowDepthMap.Get(), &dsvDesc, pDSVHeap->GetCPUDescriptorHandleForHeapStart() );
	}
}

void DirectionalLight::BeginDraw( RefPtr<CDeviceContext>& deviceContext )
{
	if ( pShadowDepthMap && pDSVHeap )
	{
		auto pCommandList = deviceContext->pCommandList.Get();
		auto dsvHandle = pDSVHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_VIEWPORT viewport{ };
		D3D12_RECT scissor{ };

		viewport.Width = 2048.0f;
		viewport.Height = 2048.0f;
		viewport.MaxDepth = 1.0f;

		scissor.right = 2048;
		scissor.bottom = 2048;

		pCommandList->OMSetRenderTargets( 0, nullptr, FALSE, &dsvHandle );
		pCommandList->RSSetViewports( 1, &viewport );
		pCommandList->RSSetScissorRects( 1, &scissor );

		deviceContext->TransitionBarrier( pShadowDepthMap.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE, 0 );
		pCommandList->ClearDepthStencilView( dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr );
	}
}

void DirectionalLight::EndDraw( RefPtr<CDeviceContext>& deviceContext )
{
	if ( pShadowDepthMap && pDSVHeap )
	{
		auto pCommandList = deviceContext->pCommandList.Get();
		deviceContext->TransitionBarrier( pShadowDepthMap.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0 );
	}
}

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::~DirectionalLight()
{
	GC.Add( pShadowDepthMap );
	GC.Add( pDSVHeap );
}

object DirectionalLight::Clone()
{
	return new DirectionalLight();
}

void DirectionalLight::Update( Time& time, Input& input )
{
	auto trp = Transform;

	auto pos = trp->Position;
	auto quat = trp->Rotation;
	auto direction = ( quat * Quaternion( Vector3::Forward, 0 ) * quat.Conjugate ).V;
	auto up = ( quat * Quaternion( Vector3::Up, 0 ) * quat.Conjugate ).V;

	auto view = XMMatrixLookToLH(
		XMVectorSet( ( float )pos.X, ( float )pos.Y, ( float )pos.Z, 0 ),
		XMVectorSet( ( float )direction.X, ( float )direction.Y, ( float )direction.Z, 0 ),
		XMVectorSet( ( float )up.X, ( float )up.Y, ( float )up.Z, 0 )
	);
	auto proj = XMMatrixOrthographicLH( ( float )width, ( float )height, 1.0f, ( float )maxDepth );

	XMStoreFloat4x4( ( XMFLOAT4X4* )&frameResource.ViewProj, view * proj );

	frameResource.DirectionalLight.Direction =
	{
		( float )direction.X,
		( float )direction.Y,
		( float )direction.Z,
		0.0f
	};

	frameResource.ShadowCast = ( int )IsShadowCast;
}

double DirectionalLight::MaxDepth_get()
{
	return maxDepth;
}

void DirectionalLight::MaxDepth_set( double value )
{
	maxDepth = value;
}

double DirectionalLight::Width_get()
{
	return width;
}

void DirectionalLight::Width_set( double value )
{
	width = value;
}

double DirectionalLight::Height_get()
{
	return height;
}

void DirectionalLight::Height_set( double value )
{
	height = value;
}