#pragma once

namespace SC
{
	/// <summary> Ư�� ��Ȳ�� �߻��� ��� ������ �� �ִ� �̺�Ʈ�� ǥ���մϴ�. </summary>
	template< class TEventArgs = void >
	struct Event : public ValueType
	{
		std::vector<EventHandler<TEventArgs>> handlers;

	public:
		/// <summary> <see cref="Event"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline Event();

		/// <summary> <see cref="Event"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline Event( const Event& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override;

		/// <summary> �̺�Ʈ�� �� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handler"> �ڵ鷯 ��ü�� �����մϴ�. </param>
		/// <returns> ��ϵ� �ڵ鷯 ID�� ��ȯ�մϴ�. </returns>
		inline int64 AddHandler( EventHandler<TEventArgs> handler );

		/// <summary> �̺�Ʈ�� �� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handler"> �ڵ鷯 ��ü�� �����մϴ�. </param>
		/// <returns> ��ϵ� �ڵ鷯 ID�� ��ȯ�մϴ�. </returns>
		inline int64 AddHandler( typename EventHandler<TEventArgs>::_MyDelegate handler );

		/// <summary> �̺�Ʈ�� ��ϵ� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handlerId"> �ڵ鷯 ID�� �����մϴ�. </param>
		inline void RemoveHandler( int64 handlerId );

		/// <summary> �̺�Ʈ�� ��ϵ� �ڵ鷯 ��ü�� ȣ���մϴ�. </summary>
		/// <param name="sender"> �̺�Ʈ ȣ�� ��ü�� �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű������� �����մϴ�. </param>
		inline void Invoke( object sender, TEventArgs args );

		/// <summary> (Visual Studio ����) ��ϵ� �̺�Ʈ �ڵ鷯 ��ü�� ������ �����ɴϴ�. </summary>
		vs_property_get( int, Count );

		/// <summary> ��ϵ� �̺�Ʈ �ڵ鷯 ��ü�� ������ �����ɴϴ�. </summary>
		[[nodiscard]] inline int Count_get();

		inline int64 operator+=( EventHandler<TEventArgs> handler );
		inline int64 operator+=( typename EventHandler<TEventArgs>::_MyDelegate handler );
		inline void operator-=( int64 handlerId );
		inline void operator()( object sender, TEventArgs args );
	};

	/// <summary> Ư�� ��Ȳ�� �߻��� ��� ������ �� �ִ� �̺�Ʈ�� ǥ���մϴ�. </summary>
	template<>
	struct Event<void> : public ValueType
	{
		std::vector<EventHandler<void>> handlers;

	public:
		/// <summary> <see cref="Event"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline Event()
		{

		}

		/// <summary> <see cref="Event"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline Event( const Event& copy )
			: handlers( copy.handlers )
		{

		}

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override
		{
			return L"SC.Event<void>";
		}

		/// <summary> �̺�Ʈ�� �� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handler"> �ڵ鷯 ��ü�� �����մϴ�. </param>
		/// <returns> ��ϵ� �ڵ鷯 ID�� ��ȯ�մϴ�. </returns>
		inline int64 AddHandler( EventHandler<void> handler )
		{
			handlers.push_back( handler );
			return handler.Id_get();
		}

		/// <summary> �̺�Ʈ�� �� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handler"> �ڵ鷯 ��ü�� �����մϴ�. </param>
		/// <returns> ��ϵ� �ڵ鷯 ID�� ��ȯ�մϴ�. </returns>
		inline int64 AddHandler( EventHandler<void>::_MyDelegate handler )
		{
			return AddHandler( EventHandler<void>( handler ) );
		}

		/// <summary> �̺�Ʈ�� ��ϵ� �ڵ鷯�� �����մϴ�. </summary>
		/// <param name="handlerId"> �ڵ鷯 ID�� �����մϴ�. </param>
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

		/// <summary> �̺�Ʈ�� ��ϵ� �ڵ鷯 ��ü�� ȣ���մϴ�. </summary>
		/// <param name="sender"> �̺�Ʈ ȣ�� ��ü�� �����մϴ�. </param>
		/// <param name="args"> �̺�Ʈ �Ű������� �����մϴ�. </param>
		inline void Invoke( object sender )
		{
			for ( int i = 0; i < Count_get(); ++i )
			{
				handlers[i]( sender );
			}
		}

		/// <summary> (Visual Studio ����) ��ϵ� �̺�Ʈ �ڵ鷯 ��ü�� ������ �����ɴϴ�. </summary>
		vs_property_get( int, Count );

		/// <summary> ��ϵ� �̺�Ʈ �ڵ鷯 ��ü�� ������ �����ɴϴ�. </summary>
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