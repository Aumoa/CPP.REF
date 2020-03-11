using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

HDRBuffer::HDRBuffer( RefPtr<CDevice>& device ) : RenderTexture2D()
	, deviceRef( device )
{
	auto pDevice = deviceRef->pDevice.Get();

	// ���� Ÿ�� ������ ���� �����մϴ�.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 1;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pRTVHeap ) ) );

	// �������� ũ�⸦ ��ȸ�մϴ�.
	rtvStride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
}

void HDRBuffer::OMSetRenderTargets( RefPtr<CDeviceContext>& deviceContext )
{
	float clear[4]{ };
	auto pCommandList = deviceContext->pCommandList.Get();

	auto rtvHandle = pRTVHeap->GetCPUDescriptorHandleForHeapStart();

	// �ڿ��� ���¸� ���� Ÿ������ �����մϴ�.
	deviceContext->TransitionBarrier(
		pHDRBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET, 0
	);

	// ���� Ÿ�� �並 �����մϴ�.
	pCommandList->OMSetRenderTargets( 1, &rtvHandle, TRUE, nullptr );

	// ���� Ÿ���� ���� �ʱ�ȭ�մϴ�.
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
	// �ڿ��� ���¸� �ȼ� ���̴� �ڿ����� �����մϴ�.
	deviceContext->TransitionBarrier(
		pHDRBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, 0
	);
}

void HDRBuffer::ResizeBuffers( uint32 width, uint32 height )
{
	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_DEFAULT };
	const D3D12_RESOURCE_STATES initState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	auto pDevice = deviceRef->pDevice.Get();

	// �⺻ �ؽ�ó ������ �����մϴ�.
	D3D12_RESOURCE_DESC textureDesc{ };
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.DepthOrArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.SampleDesc = { 1, 0 };
	textureDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// �⺻ Ŭ���� ������ �����մϴ�.
	D3D12_CLEAR_VALUE clearValue{ };

	// ���۸� �����մϴ�.
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	clearValue.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &textureDesc, initState, &clearValue, IID_PPV_ARGS( &pHDRBuffer ) ) );

	// �����ڸ� �����մϴ�.
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