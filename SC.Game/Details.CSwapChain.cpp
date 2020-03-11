using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

CSwapChain::CSwapChain() : Object()
{
	auto pDXGIFactory = GlobalVar.factory->pDXGIFactory.Get();
	auto pDevice = GlobalVar.device->pDevice.Get();
	auto pCommandQueue = GlobalVar.device->DirectQueue[0]->pCommandQueue.Get();

	// ���� ������ ������� ���� ü�� ��ü�� �����մϴ�.
	ComPtr<IDXGISwapChain1> pSwapChain;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ };
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = ( UINT )BufferCount;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	HR( pDXGIFactory->CreateSwapChainForHwnd(
		pCommandQueue,
		GlobalVar.hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&pSwapChain
	) );

	HR( pSwapChain.As<IDXGISwapChain4>( &this->pSwapChain ) );

	// RTV ������ ���� �����մϴ�.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.NumDescriptors = BufferCount;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pDescriptorHeapRTV ) ) );
	rtvStride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
}

void CSwapChain::ResizeBuffers( uint32 width, uint32 height )
{
	auto pDevice = GlobalVar.device->pDevice.Get();

	// ������ ũ�⸦ �����ϱ� �� ��� ������ �����մϴ�.
	for ( int i = 0; i < BufferCount; ++i )
	{
		ppBackBuffers[i] = nullptr;
	}

	// ������ ũ�⸦ �����մϴ�.
	HR( pSwapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 ) );

	// ���ο� ���� ������ �Բ� ���� Ÿ�� �����ڸ� �����մϴ�.
	auto handleBase = pDescriptorHeapRTV->GetCPUDescriptorHandleForHeapStart();
	for ( int i = 0; i < BufferCount; ++i )
	{
		HR( pSwapChain->GetBuffer( i, IID_PPV_ARGS( &ppBackBuffers[i] ) ) );
		pDevice->CreateRenderTargetView( ppBackBuffers[i].Get(), nullptr, handleBase );
		RTVHandle[i] = handleBase;
		handleBase.ptr += rtvStride;
	}
}

void CSwapChain::Present( int syncInterval )
{
	HR( pSwapChain->Present( ( UINT )syncInterval, 0 ) );
}

int CSwapChain::Index_get()
{
	return pSwapChain->GetCurrentBackBufferIndex();
}