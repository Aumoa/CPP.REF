using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

HDRBuffer::HDRBuffer( RefPtr<CDevice>& device ) : RenderTexture2D()
	, deviceRef( device )
{
	auto pDevice = deviceRef->pDevice.Get();

	// 렌더 타겟 서술자 힙을 생성합니다.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 1;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pRTVHeap ) ) );

	// 서술자의 크기를 조회합니다.
	rtvStride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
}

void HDRBuffer::OMSetRenderTargets( RefPtr<CDeviceContext>& deviceContext )
{
	float clear[4]{ };
	auto pCommandList = deviceContext->pCommandList.Get();

	auto rtvHandle = pRTVHeap->GetCPUDescriptorHandleForHeapStart();

	// 자원의 상태를 렌더 타겟으로 변경합니다.
	deviceContext->TransitionBarrier(
		pHDRBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0
	);

	// 렌더 타겟 뷰를 설정합니다.
	pCommandList->OMSetRenderTargets( 1, &rtvHandle, TRUE, nullptr );

	// 렌더 타겟의 색을 초기화합니다.
	pCommandList->ClearRenderTargetView( rtvHandle, clear, 0, nullptr );

	pCommandList->RSSetViewports( 1, &viewport );
	pCommandList->RSSetScissorRects( 1, &scissor );
}

void HDRBuffer::SetGraphicsRootShaderResources( RefPtr<CDeviceContext>& deviceContext )
{
	deviceContext->SetGraphicsRootShaderResources( deviceContext->Slot["HDRBuffer"], 1, ppShaderResourceViews );
}

void HDRBuffer::SetComputeRootShaderResources( RefPtr<CDeviceContext>& deviceContext )
{
	deviceContext->SetComputeRootShaderResources( deviceContext->Slot["HDRBuffer"], 1, ppShaderResourceViews );
}

void HDRBuffer::EndDraw( RefPtr<CDeviceContext>& deviceContext )
{
	// 자원의 상태를 픽셀 셰이더 자원으로 변경합니다.
	deviceContext->TransitionBarrier(
		pHDRBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0
	);
}

void HDRBuffer::ResizeBuffers( uint32 width, uint32 height )
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
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pHDRBuffer ) ) );

	// 서술자를 생성합니다.
	auto handleBase = pRTVHeap->GetCPUDescriptorHandleForHeapStart();
	pDevice->CreateRenderTargetView( pHDRBuffer.Get(), nullptr, handleBase );

	ppShaderResourceViews[0] = deviceRef->CreateShaderResourceView( pHDRBuffer.Get(), nullptr );

	viewport.Width = ( float )width;
	viewport.Height = ( float )height;
	viewport.MaxDepth = 1.0f;

	scissor.right = ( int )width;
	scissor.bottom = ( int )height;

	this->width = width;
	this->height = height;
}

uint32 HDRBuffer::Width_get()
{
	return width;
}

uint32 HDRBuffer::Height_get()
{
	return height;
}