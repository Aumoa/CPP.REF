using namespace SC;
using namespace SC::Threading;

AsyncWait::AsyncWait( void* handle )
	: native_handle( handle )
{

}

bool AsyncWait::CustomRelease( void* native_handle )
{
	return false;
}

AsyncWait::~AsyncWait()
{
	if ( native_handle && CustomRelease( native_handle ) == false )
	{
		CloseHandle( native_handle );
		native_handle = nullptr;
	}
}

AsyncWaitResult AsyncWait::WaitForSingleObject( uint32 milliseconds ) const
{
	auto state = ::WaitForSingleObject( ( HANDLE )native_handle, milliseconds );

	switch ( state )
	{
	case WAIT_OBJECT_0:
		return AsyncWaitResult::Completed;
	case WAIT_ABANDONED:
		return AsyncWaitResult::Abandoned;
	case WAIT_TIMEOUT:
		return AsyncWaitResult::Timeout;
	default:
		return AsyncWaitResult::Failed;
	}
}

void* AsyncWait::Handle_get() const
{
	return native_handle;
}