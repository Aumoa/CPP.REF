using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

CDevice::CDevice( IDXGIAdapter1* pAdapter ) : Object()
	, pAdapter( pAdapter )
{
#if defined( _DEBUG )
	// 응용 프로그램이 디버그 모드로 빌드되었을 경우:
	// Direct3D SDK에서 디버그 레이어를 활성화합니다.
	if ( ComPtr<ID3D12Debug> pDebug; SUCCEEDED( D3D12GetDebugInterface( IID_PPV_ARGS( &pDebug ) ) ) )
	{
		pDebug->EnableDebugLayer();
	}
#endif

	// Direct3D 12 디바이스 개체를 생성합니다.
	HR( D3D12CreateDevice( pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS( &pDevice ) ) );

	// 기본 커맨드 큐 목록을 구성합니다.
	DirectQueue[0] = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_DIRECT );
	DirectQueue[1] = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_DIRECT );
	DirectQueue[2] = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_DIRECT );
	DirectQueue[3] = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_DIRECT );
	CopyQueue = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_COPY );
	ComputeQueue = new CCommandQueue( this, D3D12_COMMAND_LIST_TYPE_COMPUTE );

	// 초기 서술자 스토리지를 생성합니다.
	viewStorageChain.push( new ViewStorage( this ) );

	// 초기 힙 스토리지를 생성합니다.
	heapAllocator128 = new HeapAllocator( this, 128, 8192 );
	heapAllocator256 = new HeapAllocator( this, 256, 4096 );
	heapAllocator512 = new HeapAllocator( this, 512 );

	InitializeInterop();
}

void CDevice::FreeStorage( ViewStorage* pStorage )
{
	viewStorageLocker.lock();
	viewStorageChain.push( pStorage );
	viewStorageLocker.unlock();
}

ComPtr<ID3D12RootSignature> CDevice::CreateRootSignature( const D3D12_ROOT_SIGNATURE_DESC& desc, D3D_ROOT_SIGNATURE_VERSION version )
{
	ComPtr<ID3DBlob> pBlob;
	ComPtr<ID3DBlob> pError;
	ComPtr<ID3D12RootSignature> pRootSignature;
	auto hr = D3D12SerializeRootSignature( &desc, version, &pBlob, &pError );
	if ( FAILED( hr ) )
	{
		if ( pError )
		{
			OutputDebugStringA( ( const char* )pError->GetBufferPointer() );
		}
		HR( hr );
	}

	HR( pDevice->CreateRootSignature( 0, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_PPV_ARGS( &pRootSignature ) ) );
	return move( pRootSignature );
}

RefPtr<CShaderResourceView> CDevice::CreateShaderResourceView( ID3D12Resource* pResource, const D3D12_SHADER_RESOURCE_VIEW_DESC* pSRVDesc )
{
	// 서술자 공간에서 다음 서술자의 위치를 조회합니다.
	viewStorageLocker.lock();
	auto storage = viewStorageChain.front();
	auto lockIndex = viewStorageChain.front()->Lock();
	viewStorageLocker.unlock();

	// 서술자 공간이 없을 경우 새 서술자 스토리지를 생성합니다.
	if ( lockIndex == -1 )
	{
		lock_guard<mutex> lock( viewStorageLocker );
		viewStorageChain.pop();
		
		if ( viewStorageChain.empty() )
		{
			storage = new ViewStorage( this );
			viewStorageChain.push( storage );
			lockIndex = viewStorageChain.front()->Lock();
		}
	}

	// 서술자 개체를 생성합니다.
	auto handle = storage->GetCPUHandle( lockIndex );
	var srv = new CShaderResourceView( storage, lockIndex, handle );

	// 셰이더 자원 서술자를 생성합니다.
	pDevice->CreateShaderResourceView( pResource, pSRVDesc, handle );

	// 서술자 개체를 반환합니다.
	return move( srv );
}

RefPtr<CUnorderedAccessView> CDevice::CreateUnorderedAccessView( ID3D12Resource* pResource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* pUAVDesc )
{
	// 서술자 공간에서 다음 서술자의 위치를 조회합니다.
	viewStorageLocker.lock();
	auto storage = viewStorageChain.front();
	auto lockIndex = viewStorageChain.front()->Lock();
	viewStorageLocker.unlock();

	// 서술자 공간이 없을 경우 새 서술자 스토리지를 생성합니다.
	if ( lockIndex == -1 )
	{
		lock_guard<mutex> lock( viewStorageLocker );
		viewStorageChain.pop();

		if ( viewStorageChain.empty() )
		{
			storage = new ViewStorage( this );
			viewStorageChain.push( storage );
			lockIndex = viewStorageChain.front()->Lock();
		}
	}

	// 서술자 개체를 생성합니다.
	auto handle = storage->GetCPUHandle( lockIndex );
	var srv = new CUnorderedAccessView( storage, lockIndex, handle );

	// 셰이더 자원 서술자를 생성합니다.
	pDevice->CreateUnorderedAccessView( pResource, nullptr, pUAVDesc, handle );

	// 서술자 개체를 반환합니다.
	return move( srv );
}

RefPtr<CDynamicBuffer> CDevice::CreateDynamicBuffer( uint64 sizeInBytes, int alignment )
{
	HeapAllocator* pAlloc = nullptr;

	if ( alignment == 0 )
	{
		if ( sizeInBytes <= 128 ) alignment = 128;
		else if ( sizeInBytes <= 256 ) alignment = 256;
		else if ( sizeInBytes <= 512 ) alignment = 512;
		else alignment = 0;
	}

	if ( alignment == 128 )
	{
		pAlloc = heapAllocator128.Get();
	}
	else if ( alignment == 256 )
	{
		pAlloc = heapAllocator256.Get();
	}
	else if ( alignment == 512 )
	{
		pAlloc = heapAllocator512.Get();
	}

	if ( !pAlloc || alignment < sizeInBytes )
	{
		D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };
		D3D12_RESOURCE_DESC bufferDesc{ };
		bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		bufferDesc.Width = sizeInBytes;
		bufferDesc.Height = 1;
		bufferDesc.DepthOrArraySize = 1;
		bufferDesc.MipLevels = 1;
		bufferDesc.SampleDesc = { 1, 0 };
		bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		ComPtr<ID3D12Resource> pResource;
		HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pResource ) ) );

		return new CDynamicBuffer( move( pResource ) );
	}

	D3D12_GPU_VIRTUAL_ADDRESS virAddr;
	void* pBlock;
	auto index = pAlloc->Alloc( virAddr, pBlock );
	return new CDynamicBuffer( pAlloc, index, virAddr, pBlock );
}

void CDevice::InitializeInterop()
{
	auto pCommandQueue = DirectQueue[3]->pCommandQueue.Get();

	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	D2D1_CREATION_PROPERTIES creationProperties{ };

	creationProperties.threadingMode = D2D1_THREADING_MODE_MULTI_THREADED;
	creationProperties.debugLevel = D2D1_DEBUG_LEVEL_NONE;
	creationProperties.options = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;

#if defined( _DEBUG )
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	creationProperties.debugLevel = D2D1_DEBUG_LEVEL_WARNING;
#endif

	// D3D11 호환 장치를 생성합니다.
	HR( D3D11On12CreateDevice( pDevice.Get(), deviceFlags, nullptr, 0, ( IUnknown** )&pCommandQueue, 1, 0, &pDevice11, &pDeviceContext11, nullptr ) );
	HR( pDevice11.As<ID3D11On12Device>( &pDevice11On12 ) );

	// D3D11 호환 장치에서 D2D1 장치를 생성합니다.
	ComPtr<IDXGIDevice> pDxgiDevice;
	HR( pDevice11On12.As<IDXGIDevice>( &pDxgiDevice ) );
	HR( D2D1CreateDevice( pDxgiDevice.Get(), creationProperties, &pDevice2D ) );
	HR( pDevice2D->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &pDeviceContext2D ) );
}