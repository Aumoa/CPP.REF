using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

ViewStorage::ViewStorage( CDevice* device, int capacity ) : Object()
	, deviceRef( device )
	, pDevice( device->pDevice )
	, capacity( capacity )
{
	// 서술자 힙 개체를 생성합니다.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = ( UINT )capacity;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pDescriptorHeap ) ) );
	handleBase = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// 서술자 핸들의 크기를 조회합니다.
	stride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	// 인덱스 큐에 모든 집합을 채웁니다.
	for ( int i = 0; i < capacity; ++i )
	{
		index_queue.push( i );
	}
}

int ViewStorage::Lock()
{
	if ( index_queue.empty() )
	{
		// 큐에 값이 없을 경우 실패(-1)를 반환합니다.
		return -1;
	}
	else
	{
		// 큐의 값을 반환합니다.
		locker.lock();
		auto ret = index_queue.front();
		index_queue.pop();
		locker.unlock();
		return ret;
	}
}

void ViewStorage::Free( int lockIndex )
{
	// 큐에 값을 돌려줍니다.
	locker.lock();
	if ( index_queue.empty() )
	{
		deviceRef->FreeStorage( this );
	}
	index_queue.push( lockIndex );
	locker.unlock();
}

D3D12_CPU_DESCRIPTOR_HANDLE ViewStorage::GetCPUHandle( int lockIndex )
{
#if defined( _DEBUG )
	if ( lockIndex == -1 )
	{
		throw ArgumentException( "SC.Game.Details.ViewStorage.GetCPUHandle( int lockIndex == -1 )" );
	}
#endif

	D3D12_CPU_DESCRIPTOR_HANDLE handle = handleBase;
	handle.ptr += stride * lockIndex;

	return handle;
}