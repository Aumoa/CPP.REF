#pragma once

namespace SC
{
	/// <summary> 특정 상황이 발생할 경우 구독할 수 있는 이벤트를 표현합니다. </summary>
	template< class TEventArgs = void >
	struct Event : public ValueType
	{
		std::vector<EventHandler<TEventArgs>> handlers;

	public:
		/// <summary> <see cref="Event"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline Event();

		/// <summary> <see cref="Event"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline Event( const Event& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override;

		/// <summary> 이벤트에 새 핸들러를 구독합니다. </summary>
		/// <param name="handler"> 핸들러 개체를 전달합니다. </param>
		/// <returns> 등록된 핸들러 ID를 반환합니다. </returns>
		inline int64 AddHandler( EventHandler<TEventArgs> handler );

		/// <summary> 이벤트에 새 핸들러를 구독합니다. </summary>
		/// <param name="handler"> 핸들러 개체를 전달합니다. </param>
		/// <returns> 등록된 핸들러 ID를 반환합니다. </returns>
		inline int64 AddHandler( typename EventHandler<TEventArgs>::_MyDelegate handler );

		/// <summary> 이벤트에 등록된 핸들러를 제거합니다. </summary>
		/// <param name="handlerId"> 핸들러 ID를 전달합니다. </param>
		inline void RemoveHandler( int64 handlerId );

		/// <summary> 이벤트에 등록된 핸들러 개체를 호출합니다. </summary>
		/// <param name="sender"> 이벤트 호출 개체를 전달합니다. </param>
		/// <param name="args"> 이벤트 매개변수를 전달합니다. </param>
		inline void Invoke( object sender, TEventArgs args );

		/// <summary> (Visual Studio 전용) 등록된 이벤트 핸들러 개체의 개수를 가져옵니다. </summary>
		vs_property_get( int, Count );

		/// <summary> 등록된 이벤트 핸들러 개체의 개수를 가져옵니다. </summary>
		[[nodiscard]] inline int Count_get();

		inline int64 operator+=( EventHandler<TEventArgs> handler );
		inline int64 operator+=( typename EventHandler<TEventArgs>::_MyDelegate handler );
		inline void operator-=( int64 handlerId );
		inline void operator()( object sender, TEventArgs args );
	};

	/// <summary> 특정 상황이 발생할 경우 구독할 수 있는 이벤트를 표현합니다. </summary>
	template<>
	struct Event<void> : public ValueType
	{
		std::vector<EventHandler<void>> handlers;

	public:
		/// <summary> <see cref="Event"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline Event()
		{

		}

		/// <summary> <see cref="Event"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline Event( const Event& copy )
			: handlers( copy.handlers )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override
		{
			return L"SC.Event<void>";
		}

		/// <summary> 이벤트에 새 핸들러를 구독합니다. </summary>
		/// <param name="handler"> 핸들러 개체를 전달합니다. </param>
		/// <returns> 등록된 핸들러 ID를 반환합니다. </returns>
		inline int64 AddHandler( EventHandler<void> handler )
		{
			handlers.push_back( handler );
			return handler.Id_get();
		}

		/// <summary> 이벤트에 새 핸들러를 구독합니다. </summary>
		/// <param name="handler"> 핸들러 개체를 전달합니다. </param>
		/// <returns> 등록된 핸들러 ID를 반환합니다. </returns>
		inline int64 AddHandler( EventHandler<void>::_MyDelegate handler )
		{
			return AddHandler( EventHandler<void>( handler ) );
		}

		/// <summary> 이벤트에 등록된 핸들러를 제거합니다. </summary>
		/// <param name="handlerId"> 핸들러 ID를 전달합니다. </param>
		inline void RemoveHandler( int64 handlerId )
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

		/// <summary> 이벤트에 등록된 핸들러 개체를 호출합니다. </summary>
		/// <param name="sender"> 이벤트 호출 개체를 전달합니다. </param>
		/// <param name="args"> 이벤트 매개변수를 전달합니다. </param>
		inline void Invoke( object sender )
		{
			for ( int i = 0; i < Count_get(); ++i )
			{
				handlers[i]( sender );
			}
		}

		/// <summary> (Visual Studio 전용) 등록된 이벤트 핸들러 개체의 개수를 가져옵니다. </summary>
		vs_property_get( int, Count );

		/// <summary> 등록된 이벤트 핸들러 개체의 개수를 가져옵니다. </summary>
		[[nodiscard]] inline int Count_get()
		{
			return ( int )handlers.size();
		}

		inline int64 operator+=( EventHandler<void> handler )
		{
			return AddHandler( handler );
		}

		inline int64 operator+=( EventHandler<void>::_MyDelegate handler )
		{
			return AddHandler( handler );
		}

		inline void operator-=( int64 handlerId )
		{
			return RemoveHandler( handlerId );
		}

		inline void operator()( object sender )
		{
			return Invoke( sender );
		}
	};
}