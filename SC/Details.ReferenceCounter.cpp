using namespace SC;
using namespace SC::Details;

using namespace std;

atomic<int> ReferenceCounter::allocRef;

void ReferenceCounter::AddRef()
{
	InterlockedIncrement64( ( volatile LONGLONG* )&refCount );
	InterlockedIncrement64( ( volatile LONGLONG* )&weakCount );
}

void ReferenceCounter::Release()
{
	InterlockedDecrement64( ( volatile LONGLONG* )&refCount );
	InterlockedDecrement64( ( volatile LONGLONG* )&weakCount );
}

void ReferenceCounter::AddWeakRef()
{
	InterlockedIncrement64( ( volatile LONGLONG* )&weakCount );
}

void ReferenceCounter::ReleaseWeak()
{
	InterlockedDecrement64( ( volatile LONGLONG* )&weakCount );
}

void* ReferenceCounter::operator new( size_t length )
{
	AddAllocRef();

	return GetStaticAlloc<16>().Alloc( length );
}

void ReferenceCounter::operator delete( void* block )
{
	GetStaticAlloc<16>().Free( block );

	ReleaseAllocRef();
}

void ReferenceCounter::AddAllocRef()
{
	int expected = 0;
	if ( allocRef.compare_exchange_weak( expected, 1 ) )
	{
		GetStaticAlloc<16>().Wakeup();
	}
	else
	{
		allocRef += 1;
	}
}

void ReferenceCounter::ReleaseAllocRef()
{
	int expected = 1;

	if ( allocRef.compare_exchange_weak( expected, 0 ) )
	{
		GetStaticAlloc<16>().Cleanup();
	}
	else
	{
		allocRef -= 1;
	}
}