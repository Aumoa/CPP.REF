using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

HeapAllocator::HeapAllocator( CDevice* device, int alignOf, uint32 heapCount ) : Object()
	, alignOf( alignOf )
	, heapCount( heapCount )
	, pDevice( device->pDevice )
{
	D3D12_HEAP_PROPERTIES heapProp{ D3D12_HEAP_TYPE_UPLOAD };

	D3D12_RESOURCE_DESC bufferDesc{ };
	bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	bufferDesc.Width = alignOf * heapCount;
	bufferDesc.Height = 1;
	bufferDesc.DepthOrArraySize = 1;
	bufferDesc.MipLevels = 1;
	bufferDesc.SampleDesc = { 1, 0 };
	bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HR( pDevice->CreateCommittedResource( &heapProp, D3D12_HEAP_FLAG_NONE, &bufferDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS( &pDynamicBuffer ) ) );
	HR( pDynamicBuffer->Map( 0, nullptr, ( void** )&pBlock ) );
	virtualAddress = pDynamicBuffer->GetGPUVirtualAddress();

	for ( int i = 0; i < ( int )heapCount; ++i )
	{
		allocQueue.push( i );
	}
}

int HeapAllocator::Alloc( D3D12_GPU_VIRTUAL_ADDRESS& virtualAddress, void*& pBlock )
{
	lock_guard<mutex> lock( locker );

#if defined( _DEBUG )
	if ( allocQueue.empty() )
	{
		throw new Exception( "SC.Game.Details.HeapAllocator.Alloc(): allocQueue is empty." );
	}
#endif

	auto index = allocQueue.front();
	allocQueue.pop();

	virtualAddress = this->virtualAddress + index * alignOf;
	pBlock = ( void* )( this->pBlock + index * alignOf );

	return index;
}

void HeapAllocator::Free( int index )
{
	lock_guard<mutex> lock( locker );

	allocQueue.push( index );
}