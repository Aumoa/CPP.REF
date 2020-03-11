#pragma once

namespace SC
{
	template< class TEventArgs >
	inline Event<TEventArgs>::Event()
	{

	}

	template< class TEventArgs >
	inline Event<TEventArgs>::Event( const Event& copy )
		: handlers( copy.handlers )
	{

	}

	template< class TEventArgs >
	inline String Event<TEventArgs>::ToString()
	{
		return String::Format( "SC.Event<{0}>", typeid( TEventArgs ).name() );
	}

	template< class TEventArgs >
	inline int64 Event<TEventArgs>::AddHandler( EventHandler<TEventArgs> handler )
	{
		handlers.push_back( handler );
		return handler.Id_get();
	}

	template< class TEventArgs >
	inline int64 Event<TEventArgs>::AddHandler( typename EventHandler<TEventArgs>::_MyDelegate handler )
	{
		return AddHandler( EventHandler<TEventArgs>( handler ) );
	}

	template< class TEventArgs >
	inline void Event<TEventArgs>::RemoveHandler( int64 handlerId )
	{
		for ( int i = 0; i < Count_get(); ++i )
		{
			if ( handlers[i].Id_get() == handlerId )
			{
				handlers.erase( handlers.begin() + i );
				return;
			}
		}
	}

	template< class TEventArgs >
	inline void Event<TEventArgs>::Invoke( object sender, TEventArgs args )
	{
		for ( int i = 0; i < Count_get(); ++i )
		{
			handlers[i]( sender, args );
		}
	}

	template< class TEventArgs >
	[[nodiscard]] inline int Event<TEventArgs>::Count_get()
	{
		return ( int )handlers.size();
	}

	template< class TEventArgs >
	inline int64 Event<TEventArgs>::operator+=( EventHandler<TEventArgs> handler )
	{
		return AddHandler( handler );
	}

	template< class TEventArgs >
	inline int64 Event<TEventArgs>::operator+=( typename EventHandler<TEventArgs>::_MyDelegate handler )
	{
		return AddHandler( handler );
	}

	template< class TEventArgs >
	inline void Event<TEventArgs>::operator-=( int64 handlerId )
	{
		return RemoveHandler( handlerId );
	}

	template< class TEventArgs >
	inline void Event<TEventArgs>::operator()( object sender, TEventArgs args )
	{
		return Invoke( sender, args );
	}
}