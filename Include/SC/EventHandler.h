#pragma once

namespace SC
{
	/// <summary> 이벤트 매개 변수를 포함하는 이벤트 처리기 형식을 표현합니다. </summary>
	template< class TEventArgs = void >
	struct EventHandler : public Delegate
	{
	public:
		using _MyDelegate = std::function<void( object sender, TEventArgs args )>;

	private:
		_MyDelegate callback;

	public:
		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline EventHandler() = default;

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		inline EventHandler( std::function<void( object sender, TEventArgs args )> callback );

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		template< class T >
		inline EventHandler( T* caller, void( T::*callback )( object sender, TEventArgs args ) );

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		template< class T >
		inline EventHandler( RefPtr<T> caller, void( T::*callback )( object sender, TEventArgs args ) );

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline EventHandler( const EventHandler& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override;

		/// <summary> 이벤트 처리기 함수를 호출합니다. </summary>
		/// <param name="sender"> 호출자 개체를 전달합니다. </param>
		/// <param name="args"> 이벤트 매개 변수를 전달합니다. </param>
		inline void Invoke( object sender, TEventArgs args );

		/// <summary> (Visual Studio 전용) 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		vs_property( bool, IsValid );

		/// <summary> 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		inline bool IsValid_get();

		/// <summary> (Visual Studio 전용) 연결된 대리자 개체의 ID를 가져옵니다. </summary>
		vs_property( int64, Id );

		/// <summary> 연결된 대리자 개체의 ID를 가져옵니다. </summary>
		inline int64 Id_get();

		inline void operator()( object sender, TEventArgs args );
		inline EventHandler& operator=( const EventHandler& copy );
		inline bool operator==( const EventHandler& right ) const;
	};

	/// <summary> 이벤트 매개 변수를 포함하는 이벤트 처리기 형식을 표현합니다. </summary>
	template<>
	struct EventHandler<void> : public Delegate
	{
	public:
		using _MyDelegate = std::function<void( object sender )>;

	private:
		_MyDelegate callback;

	public:
		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline EventHandler() = default;

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		inline EventHandler( std::function<void( object sender )> callback )
			: callback( callback )
		{

		}

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		template< class T >
		inline EventHandler( T* caller, void( T::* callback )( object sender ) )
			: callback( [caller, callback]( object sender ) { ( caller->*callback )( sender ); } )
		{

		}

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="caller"> 이벤트 처리기 함수를 호출하는 소유자 개체를 전달합니다. </param>
		/// <param name="callback"> 이벤트 처리기 함수를 전달합니다. </param>
		template< class T >
		inline EventHandler( RefPtr<T> caller, void( T::* callback )( object sender ) )
			: callback( [caller, callback]( object sender ) { ( caller->*callback )( sender ); } )
		{

		}

		/// <summary> <see cref="EventHandler"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		inline EventHandler( const EventHandler& copy )
			: callback( copy.callback )
		{

		}

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override
		{
			return L"SC.Event<void>";
		}

		/// <summary> 이벤트 처리기 함수를 호출합니다. </summary>
		/// <param name="sender"> 호출자 개체를 전달합니다. </param>
		/// <param name="args"> 이벤트 매개 변수를 전달합니다. </param>
		inline void Invoke( object sender )
		{
			callback( sender );
		}

		/// <summary> (Visual Studio 전용) 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		vs_property( bool, IsValid );

		/// <summary> 연결된 대리자 개체가 유효한지 검사합니다. </summary>
		inline bool IsValid_get()
		{
			if ( callback ) return true;
			else return false;
		}

		/// <summary> (Visual Studio 전용) 연결된 대리자 개체의 ID를 가져옵니다. </summary>
		vs_property( int64, Id );

		/// <summary> 연결된 대리자 개체의 ID를 가져옵니다. </summary>
		inline int64 Id_get()
		{
			return ( int64 )callback.target<void( object )>();
		}

		inline void operator()( object sender )
		{
			callback( sender );
		}

		inline EventHandler& operator=( const EventHandler& copy )
		{
			callback = copy.callback;
			return *this;
		}

		inline bool operator==( const EventHandler& right ) const
		{
			return callback.target<void( object )>() == right.callback.target<void( object )>();
		}
	};
}