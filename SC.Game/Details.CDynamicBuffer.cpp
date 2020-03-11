using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

CDynamicBuffer::CDynamicBuffer( RefPtr<HeapAllocator> pAllocator, int indexOf, D3D12_GPU_VIRTUAL_ADDRESS virtualAddress, void* pBlock ) : Object()
	, indexOf( indexOf )
	, pAllocator( pAllocator )
	, VirtualAddress( virtualAddress )
	, pBlock( pBlock )
{

}

CDynamicBuffer::CDynamicBuffer( ComPtr<ID3D12Resource>&& pResource ) : Object()
	, indexOf( -1 )
	, pResource( move( pResource ) )
{
	HR( this->pResource->Map( 0, nullptr, &pBlock ) );
	VirtualAddress = this->pResource->GetGPUVirtualAddress();
}

CDynamicBuffer::~CDynamicBuffer()
{
	if ( indexOf != -1 )
		pAllocator->Free( indexOf );
}