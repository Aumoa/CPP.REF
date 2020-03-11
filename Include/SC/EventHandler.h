#pragma once

namespace SC
{
	/// <summary> �̺�Ʈ �Ű� ������ �����ϴ� �̺�Ʈ ó���� ������ ǥ���մϴ�. </summary>
	template< class TEventArgs = void >
	struct EventHandler : public Delegate
	{
	public:
		using _MyDelegate = std::function<void( object sender, TEventArgs args )>;

	private:
		_MyDelegate callback;

	public:
		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline EventHandler() = default;

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		inline EventHandler( std::function<void( object sender, TEventArgs args )> callback );

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		template< class T >
		inline EventHandler( T* caller, void( T::*callback )( object sender, TEventArgs args ) );

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		template< class T >
		inline EventHandler( RefPtr<T> caller, void( T::*callback )( object sender, TEventArgs args ) );

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline EventHandler( const EventHandler& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override;

		/// <summary> �̺�Ʈ ó���� �Լ��� ȣ���մϴ�. </summary>
		/// <param name="sender"> ȣ���� ��ü�� �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű� ������ �����մϴ�. </param>
		inline void Invoke( object sender, TEventArgs args );

		/// <summary> (Visual Studio ����) ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		vs_property( bool, IsValid );

		/// <summary> ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		inline bool IsValid_get();

		/// <summary> (Visual Studio ����) ����� �븮�� ��ü�� ID�� �����ɴϴ�. </summary>
		vs_property( int64, Id );

		/// <summary> ����� �븮�� ��ü�� ID�� �����ɴϴ�. </summary>
		inline int64 Id_get();

		inline void operator()( object sender, TEventArgs args );
		inline EventHandler& operator=( const EventHandler& copy );
		inline bool operator==( const EventHandler& right ) const;
	};

	/// <summary> �̺�Ʈ �Ű� ������ �����ϴ� �̺�Ʈ ó���� ������ ǥ���մϴ�. </summary>
	template<>
	struct EventHandler<void> : public Delegate
	{
	public:
		using _MyDelegate = std::function<void( object sender )>;

	private:
		_MyDelegate callback;

	public:
		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline EventHandler() = default;

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		inline EventHandler( std::function<void( object sender )> callback )
			: callback( callback )
		{

		}

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		template< class T >
		inline EventHandler( T* caller, void( T::* callback )( object sender ) )
			: callback( [caller, callback]( object sender ) { ( caller->*callback )( sender ); } )
		{

		}

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="caller"> �̺�Ʈ ó���� �Լ��� ȣ���ϴ� ������ ��ü�� �����մϴ�. </param>
		/// <param name="callback"> �̺�Ʈ ó���� �Լ��� �����մϴ�. </param>
		template< class T >
		inline EventHandler( RefPtr<T> caller, void( T::* callback )( object sender ) )
			: callback( [caller, callback]( object sender ) { ( caller->*callback )( sender ); } )
		{

		}

		/// <summary> <see cref="EventHandler"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline EventHandler( const EventHandler& copy )
			: callback( copy.callback )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override
		{
			return L"SC.Event<void>";
		}

		/// <summary> �̺�Ʈ ó���� �Լ��� ȣ���մϴ�. </summary>
		/// <param name="sender"> ȣ���� ��ü�� �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű� ������ �����մϴ�. </param>
		inline void Invoke( object sender )
		{
			callback( sender );
		}

		/// <summary> (Visual Studio ����) ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		vs_property( bool, IsValid );

		/// <summary> ����� �븮�� ��ü�� ��ȿ���� �˻��մϴ�. </summary>
		inline bool IsValid_get()
		{
			if ( callback ) return true;
			else return false;
		}

		/// <summary> (Visual Studio ����) ����� �븮�� ��ü�� ID�� �����ɴϴ�. </summary>
		vs_property( int64, Id );

		/// <summary> ����� �븮�� ��ü�� ID�� �����ɴϴ�. </summary>
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