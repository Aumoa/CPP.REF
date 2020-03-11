#pragma once

namespace SC::Diagnostics
{
	template< class T >
	inline StepTimerCallbackDelegate::StepTimerCallbackDelegate( T* caller, void( T::*callback )( RefPtr<StepTimer> sender ) )
		: StepTimerCallbackDelegate( [caller, callback]( RefPtr<StepTimer> sender ) { return ( caller->*callback )( sender ); } )
	{

	}

	template< class T >
	inline StepTimerCallbackDelegate::StepTimerCallbackDelegate( RefPtr<T> caller, void( T::*callback )( RefPtr<StepTimer> sender ) )
		: StepTimerCallbackDelegate( [caller, callback]( RefPtr<StepTimer> sender ) { return ( caller->*callback )( sender ); } )
	{

	}
}