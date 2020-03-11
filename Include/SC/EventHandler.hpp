#pragma once

namespace SC
{
	/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
	template< class TEventArgs >
	inline EventHandler<TEventArgs>::EventHandler( std::function<void( object sender, TEventArgs args )> callback )
		: callback( callback )
	{

	}

	/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
	/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
	template< class TEventArgs >
	template< class T >
	inline EventHandler<TEventArgs>::EventHandler( T* caller, void( T::*callback )( object sender, TEventArgs args ) )
		: callback( [caller, callback]( object sender, TEventArgs args ) { ( caller->*callback )( sender, args ); } )
	{

	}

	/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
	/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
	template< class TEventArgs >
	template< class T >
	inline EventHandler<TEventArgs>::EventHandler( RefPtr<T> caller, void( T::*callback )( object sender, TEventArgs args ) )
		: callback( [caller, callback]( object sender, TEventArgs args ) { ( caller->*callback )( sender, args ); } )
	{

	}

	/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class TEventArgs >
	inline EventHandler<TEventArgs>::EventHandler( const EventHandler& copy )
		: callback( copy.callback )
	{

	}

	/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
	template< class TEventArgs >
	inline String EventHandler<TEventArgs>::ToString()
	{
		return String::Format( "SC.EventHandler<{0}>", typeid( TEventArgs ).name() );
	}

	/// <summary> �̺�Ʈ ó���� �Լ��� ȣ���մϴ�. </summary>
	/// <param name="sender"> ȣ���� ��ü�� �����մϴ�. </param>
	/// <param name="args"> �̺�Ʈ �Ű� ������ �����մϴ�. </param>
	template< class TEventArgs >
	inline void EventHandler<TEventArgs>::Invoke( object sender, TEventArgs args )
	{
		callback( sender, args );
	}

	/// <summary> ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
	template< class TEventArgs >
	inline bool EventHandler<TEventArgs>::IsValid_get()
	{
		if ( callback ) return true;
		else return false;
	}

	/// <summary> ����� �븮�� ��ü�� ID�� �����ɴϴ�. </summary>
	template< class TEventArgs >
	inline int64 EventHandler<TEventArgs>::Id_get()
	{
		return ( int64 )callback.target<void( object, TEventArgs )>();
	}

	template< class TEventArgs >
	inline void EventHandler<TEventArgs>::operator()( object sender, TEventArgs args )
	{
		callback( sender, args );
	}

	template< class TEventArgs >
	inline auto EventHandler<TEventArgs>::operator=( const EventHandler& copy ) -> EventHandler&
	{
		callback = copy.callback;
		return *this;
	}

	template< class TEventArgs >
	inline bool EventHandler<TEventArgs>::operator==( const EventHandler& right ) const
	{
		return callback.target<void( object, TEventArgs )>() == right.callback.target<void( object, TEventArgs )>();
	}
}