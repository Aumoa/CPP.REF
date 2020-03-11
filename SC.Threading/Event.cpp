namespace SC::Threading
{
	Event::Event( bool isManual )
		: AsyncWait( ( void* )CreateEventExW( nullptr, nullptr, isManual ? CREATE_EVENT_MANUAL_RESET : 0, GENERIC_ALL ) )
		, isManual( isManual )
	{

	}

	void Event::Set()
	{
		SetEvent( ( HANDLE )Handle );
	}

	void Event::Reset()
	{
		ResetEvent( ( HANDLE )Handle );
	}

	bool Event::IsSignaled_get() const
	{
		bool status = WaitForSingleObject( 0 ) == AsyncWaitResult::Completed;

		if ( status && isManual )
		{
			// Force mutable for comply with interface.
			( ( Event* )this )->Set();
		}

		return status;
	}
}