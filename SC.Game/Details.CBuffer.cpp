using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

CBuffer::CBuffer( RefPtr<CDevice>& device, uint64 sizeInBytes, D3D12_RESOURCE_STATES initialState, D3D12_RESOURCE_FLAGS resourceFlags, const void* pInitialData, uint64 initialDataSize ) : Object()
	, initialState( initialState )
	, deviceRef( device )
{
	// 초기 데이터가 있으면 복사 상태로, 없으면 초기 상태로 생성합니다.
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

	// 버퍼 자원을 생성합니다.
	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, createState, nullptr, IID_PPV_ARGS( &pResource ) ) );

	if ( pInitialData )
	{
		// 초기 데이터가 지정되었지만 크기가 지정되지 않았을 경우 버퍼의 크기로 설정합니다.
		if ( initialDataSize == 0 ) initialDataSize = sizeInBytes;

		// 초기 데이터를 복사할 업로드 공간을 생성합니다.
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pUploadHeap ) ) );

		// 데이터를 업로드 힙에 복사합니다.
		void* pBlock;
		HR( pUploadHeap->Map( 0, nullptr, &pBlock ) );
		memcpy( pBlock, pInitialData, ( size_t )initialDataSize );
		pUploadHeap->Unmap( 0, nullptr );

		// 데이터 업로드 명령을 수행할 명령 목록을 구성합니다.
		HR( pDevice->CreateCommandAllocator( D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS( &pUploadCommands ) ) );
		var deviceContext = new CDeviceContext( device, D3D12_COMMAND_LIST_TYPE_COPY, pUploadCommands.Get() );
		deviceContext->pCommandList->CopyResource( pResource.Get(), pUploadHeap.Get() );

		// 디바이스의 복사 큐에 명령을 제출합니다.
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
		// 리소스 상태를 초기 상태로 변경합니다.
		deviceContext->TransitionBarrier( pResource.Get(), D3D12_RESOURCE_STATE_COMMON, initialState, 0 );
		hasTransitionFlag = true;
	}

	if ( !copySuccessFlag )
	{
		// 복사가 아직 완료되지 않았을 경우,
		if ( auto pFence = deviceRef->CopyQueue->pFence.Get(); pFence->GetCompletedValue() < uploadFenceValue )
		{
			if ( waitingFlag == false && sync )
			{
				// 대상 큐에서 복사 작업이 완료될 때까지 대기하라고 지시합니다.
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