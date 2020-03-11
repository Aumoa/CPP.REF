using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

HDRComputedBuffer::HDRComputedBuffer( RefPtr<CDevice>& device ) : Object()
	, deviceRef( device )
{

}

void HDRComputedBuffer::SetComputeRootTimestamp( RefPtr<CDeviceContext>& deviceContext, double time )
{
	auto pCommandList = deviceContext->pCommandList.Get();

	if ( auto slot = deviceContext->Slot["Timer"]; slot != -1 )
	{
		float t = ( float )time;
		pCommandList->SetComputeRoot32BitConstant( ( UINT )slot, ( UINT& )t, 0 );
	}
}

void HDRComputedBuffer::SetGraphicsRootConstantBuffers( RefPtr<CDeviceContext>& deviceContext )
{
	auto pCommandList = deviceContext->pCommandList.Get();

	if ( auto slot = deviceContext->Slot["HDRConstants"]; slot != -1 )
	{
		pCommandList->SetGraphicsRootConstantBufferView( ( UINT )slot, pHDRConstants->GetGPUVirtualAddress() );
	}
}

void HDRComputedBuffer::Compute( RefPtr<CDeviceContext>& deviceContext, int pass )
{
	auto pCommandList = deviceContext->pCommandList.Get();

	if ( pass == 0 )
	{
		deviceContext->TransitionBarrier(
			pHDRConstants.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, 0
		);
		deviceContext->SetComputeRootUnorderedAccessViews( deviceContext->Slot["HDRComputedBuffer"], 2, pUnorderedAccessViews );

		uint32 dispatchX = ( ( uint32 )halfSize.X - 1 ) / 32 + 1;
		uint32 dispatchY = ( ( uint32 )halfSize.Y - 1 ) / 32 + 1;
		pCommandList->Dispatch( dispatchX, dispatchY, 1 );
	}
	else if ( pass == 1 )
	{
		uint32 dispatchX = 1;
		uint32 dispatchY = halfSize.Y;
		pCommandList->Dispatch( dispatchX, dispatchY, 1 );
	}
	else if ( pass == 2 )
	{
		uint32 dispatchX = 1;
		uint32 dispatchY = 1;
		pCommandList->Dispatch( dispatchX, dispatchY, 1 );

		deviceContext->TransitionBarrier(
			pHDRConstants.Get(), D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, 0
		);
	}
#if defined( _DEBUG )
	else
	{
		throw new ArgumentException( "SC.Game.Details.HDRComputedBuffer.Compute(): The second argument is invalid." );
	}
#endif
}

void HDRComputedBuffer::ResizeBuffers( uint32 width, uint32 height )
{
	auto pDevice = deviceRef->pDevice.Get();

	uint32 halfWidth = ( width - 1 ) / 2 + 1;
	uint32 halfHeight = ( height - 1 ) / 2 + 1;

	uint64 constantsSize =
		sizeof( HDRConstants ) +
		sizeof( float ) * halfHeight;

	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_DEFAULT };

	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = constantsSize;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	D3D12_RESOURCE_DESC textureDesc = bufferDesc;
	textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureDesc.Width = halfWidth;
	textureDesc.Height = halfHeight;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	HR( pDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS( &pHDRHalfBuffer )
	) );

	HR( pDevice->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		nullptr,
		IID_PPV_ARGS( &pHDRConstants )
	) );

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{ };
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = halfHeight + 1;
	uavDesc.Buffer.StructureByteStride = sizeof( float );

	pUnorderedAccessViews[0] = deviceRef->CreateUnorderedAccessView( pHDRHalfBuffer.Get(), nullptr );
	pUnorderedAccessViews[1] = deviceRef->CreateUnorderedAccessView( pHDRConstants.Get(), &uavDesc );

	halfSize.X = halfWidth;
	halfSize.Y = halfHeight;
}