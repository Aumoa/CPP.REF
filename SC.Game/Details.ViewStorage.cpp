using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

ViewStorage::ViewStorage( CDevice* device, int capacity ) : Object()
	, deviceRef( device )
	, pDevice( device->pDevice )
	, capacity( capacity )
{
	// ������ �� ��ü�� �����մϴ�.
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{ };
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = ( UINT )capacity;
	HR( pDevice->CreateDescriptorHeap( &heapDesc, IID_PPV_ARGS( &pDescriptorHeap ) ) );
	handleBase = pDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	// ������ �ڵ��� ũ�⸦ ��ȸ�մϴ�.
	stride = pDevice->GetDescriptorHandleIncrementSize( D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV );

	// �ε��� ť�� ��� ������ ä��ϴ�.
	for ( int i = 0; i < capacity; ++i )
	{
		index_queue.push( i );
	}
}

int ViewStorage::Lock()
{
	if ( index_queue.empty() )
	{
		// ť�� ���� ���� ��� ����(-1)�� ��ȯ�մϴ�.
		return -1;
	}
	else
	{
		// ť�� ���� ��ȯ�մϴ�.
		locker.lock();
		auto ret = index_queue.front();
		index_queue.pop();
		locker.unlock();
		return ret;
	}
}

void ViewStorage::Free( int lockIndex )
{
	// ť�� ���� �����ݴϴ�.
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