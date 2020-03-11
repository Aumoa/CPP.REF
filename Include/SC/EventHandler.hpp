#pragma once

namespace SC
{
	/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
	/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
	template< class TEventArgs >
	inline EventHandler<TEventArgs>::EventHandler( std::function<void( object sender, TEventArgs args )> callback )
		: callback( callback )
	{

	}

	/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
	/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
	/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
	template< class TEventArgs >
	template< class T >
	inline EventHandler<TEventArgs>::EventHandler( T* caller, void( T::*callback )( object sender, TEventArgs args ) )
		: callback( [caller, callback]( object sender, TEventArgs args ) { ( caller->*callback )( sender, args ); } )
	{

	}

	/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
	/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
	/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
	template< class TEventArgs >
	template< class T >
	inline EventHandler<TEventArgs>::EventHandler( RefPtr<T> caller, void( T::*callback )( object sender, TEventArgs args ) )
		: callback( [caller, callback]( object sender, TEventArgs args ) { ( caller->*callback )( sender, args ); } )
	{

	}

	/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
	template< class TEventArgs >
	inline EventHandler<TEventArgs>::EventHandler( const EventHandler& copy )
		: callback( copy.callback )
	{

	}

	/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
	template< class TEventArgs >
	inline String EventHandler<TEventArgs>::ToString()
	{
		return String::Format( "SC.EventHandler<{0}>", typeid( TEventArgs ).name() );
	}

	/// <summary> 이벤트 처리기 함수를 호출합니다. </summary>
	/// <param name="sender"> 호출자 개체를 전달합니다. </param>
	/// <param name="args"> 이벤트 매개 변수를 전달합니다. </param>
	template< class TEventArgs >
	inline void EventHandler<TEventArgs>::Invoke( object sender, TEventArgs args )
	{
		callback( sender, args );
	}

	/// <summary> 연결된 대리자 개체가 유효한지 검사합니다. </summary>
	template< class TEventArgs >
	inline bool EventHandler<TEventArgs>::IsValid_get()
	{
		if ( callback ) return true;
		else return false;
	}

	/// <summary> 연결된 대리자 개체의 ID를 가져옵니다. </summary>
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