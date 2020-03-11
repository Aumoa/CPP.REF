#pragma once

namespace SC
{
	/// <summary> ���� Ƚ�� ���濡 ���� ��ü ���Ÿ� ����ϸ鼭 ��ü�� �����ϴ� ���� ���� �����͸� ǥ���մϴ�. </summary>
	struct WeakPtr : public ValueType
	{
	private:
		Object* ptr = nullptr;
		Details::ReferenceCounter* counter = nullptr;

	public:
		/// <summary> <see cref="WeakPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		WeakPtr( Object* ptr = nullptr );
		~WeakPtr() override;

		/// <summary> <see cref="WeakPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		template< class T >
		inline WeakPtr( RefPtr<T>& ptr )
			: WeakPtr( ptr.Get() )
		{

		}

		/// <summary> <see cref="WeakPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		template< class T >
		inline WeakPtr( const RefPtr<T>& ptr )
			: WeakPtr( ( RefPtr<T>& )ptr )
		{

		}

		/// <summary> <see cref="WeakPtr"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		WeakPtr( const WeakPtr& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ����Ű�� ��ü�� ������ ��� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		String ToString() override;

		/// <summary> ��ü�� ���� �����͸� �����ɴϴ�. </summary>
		/// <exception cref="NullReferenceException"> ��ü�� �̹� ���ŵ� ��ü�� ��� �߻��մϴ�. </exception>
		/// <exception cref="InvalidCastException"> ��ü�� ������ ������ ���İ� ��ġ���� ���� ��� �߻��մϴ�. </exception>
		template< class T >
		inline RefPtr<remove_refptr_t<T>> ResolveAs()
		{
			return Resolve().As<T>();
		}

		/// <summary> ��ü�� ���� �����͸� �����ɴϴ�. ������ ��� null�� ��ȯ�մϴ�. </summary>
		template< class T >
		inline RefPtr<remove_refptr_t<T>> TryResolveAs()
		{
			auto obj = Resolve();
			if ( obj.IsValid_get() )
			{
				if ( RefPtr<remove_refptr_t<T>> obj2; obj.Is<remove_refptr_t<T>>( &obj2 ) )
				{
					return obj2;
				}
			}
			return nullptr;
		}

		/// <summary> (Visual Studio ����) ��ü�� ���� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		vs_property_get( bool, IsValid );

		/// <summary> ��ü�� ���� ���θ� ��Ÿ���� ���� �����ɴϴ�. </summary>
		bool IsValid_get();

		WeakPtr& operator=( const WeakPtr& copy );

	private:
		void AddRef();
		void Release();
		object Resolve();
	};
}