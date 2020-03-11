using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

CBuffer::CBuffer( RefPtr<CDevice>& device, uint64 sizeInBytes, D3D12_RESOURCE_STATES initialState, D3D12_RESOURCE_FLAGS resourceFlags, const void* pInitialData, uint64 initialDataSize ) : Object()
	, initialState( initialState )
	, deviceRef( device )
{
	// �ʱ� �����Ͱ� ������ ���� ���·�, ������ �ʱ� ���·� �����մϴ�.
	auto createState = pInitialData ? D3D12_RESOURCE_STATE_COMMON : initialState;
	auto pDevice = device->pDevice.Get();

	D3D12_HEAP_PROPERTIES heapProp{ };
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = sizeInBytes;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	bufferDesc.Flags = resourceFlags;

	// ���� �ڿ��� �����մϴ�.
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, createState, nullptr, IID_PPV_ARGS( &pResource ) ) );

	if ( pInitialData )
	{
		// �ʱ� �����Ͱ� �����Ǿ����� ũ�Ⱑ �������� �ʾ��� ��� ������ ũ��� �����մϴ�.
		if ( initialDataSize == 0 ) initialDataSize = sizeInBytes;

		// �ʱ� �����͸� ������ ���ε� ������ �����մϴ�.
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pUploadHeap ) ) );

		// �����͸� ���ε� ���� �����մϴ�.
		void* pBlock;
		HR( pUploadHeap->Map( 0, nullptr, &pBlock ) );
		memcpy( pBlock, pInitialData, ( size_t )initialDataSize );
		pUploadHeap->Unmap( 0, nullptr );

		// ������ ���ε� ����� ������ ��� ����� �����մϴ�.
		HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS( &pUploadCommands ) ) );
		var deviceContext = new CDeviceContext( device, D3D12_COMMAND_LIST_TYPE_COPY, pUploadCommands.Get() );
		deviceContext->pCommandList->CopyResource( pResource.Get(), pUploadHeap.Get() );

		// ����̽��� ���� ť�� ����� �����մϴ�.
		deviceContext->Close();
		device->CopyQueue->Execute( deviceContext );
		uploadFenceValue = device->CopyQueue->Signal();

		copySuccessFlag = false;
	}
}

CBuffer::~CBuffer()
{
	GC.Add( pUploadCommands );
	GC.Add( pUploadHeap );
}

void CBuffer::Lock( RefPtr<CDeviceContext>& deviceContext, bool sync )
{
	if ( !hasTransitionFlag )
	{
		// ���ҽ� ���¸� �ʱ� ���·� �����մϴ�.
		deviceContext->TransitionBarrier( pResource.Get(), D3D12_RESOURCE_STATE_COMMON, initialState, 0 );
		hasTransitionFlag = true;
	}

	if ( !copySuccessFlag )
	{
		// ���簡 ���� �Ϸ���� �ʾ��� ���,
		if ( auto pFence = deviceRef->CopyQueue->pFence.Get(); pFence->GetCompletedValue() < uploadFenceValue )
		{
			if ( waitingFlag == false && sync )
			{
				// ��� ť���� ���� �۾��� �Ϸ�� ������ ����϶�� �����մϴ�.
				HR( deviceContext->pCommandQueue->pCommandQueue->Wait( pFence, uploadFenceValue ) );
				waitingFlag = true;
			}
		}
		else
		{
			copySuccessFlag = true;
			pUploadCommands = nullptr;
			pUploadHeap = nullptr;
		}
	}
}