using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

GeometryBuffer::GeometryBuffer( RefPtr<CDevice>& device ) : RenderTexture2D()
	, deviceRef( device )
{
	auto pDevice = deviceRef->pDevice.Get();

	// 렌더 타겟 서술자 힙을 생성합니다.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 3;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pRTVHeap ) ) );

	// 깊이 스텐실 서술자 힙을 생성합니다.
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	heapDesc.NumDescriptors = 1;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pDSVHeap ) ) );

	// 서술자의 크기를 조회합니다.
	rtvStride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
}

void GeometryBuffer::OMSetRenderTargets( RefPtr<CDeviceContext>& deviceContext )
{
	float clear[4]{ };
	auto pCommandList = deviceContext->pCommandList.Get();

	auto rtvHandle = pRTVHeap->GetCPUDescriptorHandleForHeapStart();
	auto dsvHandle = pDSVHeap->GetCPUDescriptorHandleForHeapStart();

	// 자원의 상태를 렌더 타겟으로 변경합니다.
	deviceContext->TransitionBarrier(
		pColorBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0,
		pNormalBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0,
		pMaterialBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0,
		pDepthBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE, 0
	);

	// 렌더 타겟 뷰를 설정합니다.
	pCommandList->OMSetRenderTargets( 3, &rtvHandle, TRUE, &dsvHandle );

	// 렌더 타겟의 색을 초기화합니다.
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );
	rtvHandle.ptr += rtvStride;
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );
	rtvHandle.ptr += rtvStride;
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );
	pCommandList->ClearDepthStencilView( dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr );

	pCommandList->RSSetViewports( 1, &viewport );
	pCommandList->RSSetScissorRects( 1, &scissor );
}

void GeometryBuffer::SetGraphicsRootShaderResourcesForColor( RefPtr<CDeviceContext>& deviceContext )
{
	deviceContext->SetGraphicsRootShaderResources( deviceContext->Slot["GeometryBuffer"], 1, ppShaderResourceViews + 0 );
}

void GeometryBuffer::SetGraphicsRootShaderResourcesForLayer( RefPtr<CDeviceContext>& deviceContext )
{
	deviceContext->SetGraphicsRootShaderResources( deviceContext->Slot["GeometryBuffer"], 3, ppShaderResourceViews + 1 );
}

void GeometryBuffer::EndDraw( RefPtr<CDeviceContext>& deviceContext )
{
	// 자원의 상태를 픽셀 셰이더 자원으로 변경합니다.
	deviceContext->TransitionBarrier(
		pColorBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0,
		pNormalBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0,
		pMaterialBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0,
		pDepthBuffer.Get(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0
	);
}

void GeometryBuffer::ResizeBuffers( uint32 width, uint32 height )
{
	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_DEFAULT };
	const D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	auto pDevice = deviceRef->pDevice.Get();

	// 기본 텍스처 정보를 생성합니다.
	D3D12_RESOURCE_DESC textureDesc{ };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// 기본 클리어 정보를 생성합니다.
	D3D12_CLEAR_VALUE clearValue{ };

	// 버퍼를 생성합니다.
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pColorBuffer ) ) );

	textureDesc.Format = DXGI_FORMAT_R11G11B10_FLOAT;
	clearValue.Format = DXGI_FORMAT_R11G11B10_FLOAT;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pNormalBuffer ) ) );

	textureDesc.Format = DXGI_FORMAT_R16_UINT;
	clearValue.Format = DXGI_FORMAT_R16_UINT;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pMaterialBuffer ) ) );

	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pDepthBuffer ) ) );

	// 깊이 스텐실 버퍼의 깊이 스텐실 뷰 정보를 생성합니다.
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{ DXGI_FORMAT_D32_FLOAT, D3D12_DSV_DIMENSION_TEXTURE2D, D3D12_DSV_FLAG_NONE };
	dsvDesc.Texture2D = { };

	// 깊이 스텐실 버퍼의 셰이더 자원 뷰 정보를 생성합니다.
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{ DXGI_FORMAT_R32_FLOAT, D3D12_SRV_DIMENSION_TEXTURE2D, D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING };
	srvDesc.Texture2D = { 0, 1, 0, 0.0f };

	// 서술자를 생성합니다.
	auto handleBase = pRTVHeap->GetCPUDescriptorHandleForHeapStart();
	pDevice->CreateRenderTargetView( pColorBuffer.Get(), nullptr, handleBase );

	handleBase.ptr += rtvStride;
	pDevice->CreateRenderTargetView( pNormalBuffer.Get(), nullptr, handleBase );

	handleBase.ptr += rtvStride;
	pDevice->CreateRenderTargetView( pMaterialBuffer.Get(), nullptr, handleBase );

	handleBase = pDSVHeap->GetCPUDescriptorHandleForHeapStart();
	pDevice->CreateDepthStencilView( pDepthBuffer.Get(), &dsvDesc, handleBase );

	ppShaderResourceViews[0] = deviceRef->CreateShaderResourceView( pColorBuffer.Get(), nullptr );
	ppShaderResourceViews[1] = deviceRef->CreateShaderResourceView( pNormalBuffer.Get(), nullptr );
	ppShaderResourceViews[2] = deviceRef->CreateShaderResourceView( pMaterialBuffer.Get(), nullptr );
	ppShaderResourceViews[3] = deviceRef->CreateShaderResourceView( pDepthBuffer.Get(), &srvDesc );

	viewport.Width = ( float )width;
	viewport.Height = ( float )height;
	viewport.MaxDepth = 1.0f;

	scissor.right = ( int )width;
	scissor.bottom = ( int )height;

	this->width = width;
	this->height = height;
}

uint32 GeometryBuffer::Width_get()
{
	return width;
}

uint32 GeometryBuffer::Height_get()
{
	return height;
}