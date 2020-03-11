#pragma once

namespace SC
{
	/// <summary> 참조 횟수 변경에 의한 개체 제거를 허용하면서 개체를 참조하는 약한 참조 포인터를 표현합니다. </summary>
	struct WeakPtr : public ValueType
	{
	private:
		Object* ptr = nullptr;
		Details::ReferenceCounter* counter = nullptr;

	public:
		/// <summary> <see cref="WeakPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		WeakPtr( Object* ptr = nullptr );
		~WeakPtr() override;

		/// <summary> <see cref="WeakPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		template< class T >
		inline WeakPtr( RefPtr<T>& ptr )
			: WeakPtr( ptr.Get() )
		{

		}

		/// <summary> <see cref="WeakPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		template< class T >
		inline WeakPtr( const RefPtr<T>& ptr )
			: WeakPtr( ( RefPtr<T>& )ptr )
		{

		}

		/// <summary> <see cref="WeakPtr"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		WeakPtr( const WeakPtr& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 가리키는 개체가 존재할 경우 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		String ToString() override;

		/// <summary> 개체의 참조 포인터를 가져옵니다. </summary>
		/// <exception cref="NullReferenceException"> 개체가 이미 제거된 개체일 경우 발생합니다. </exception>
		/// <exception cref="InvalidCastException"> 개체의 형식이 제공된 형식과 일치하지 않을 경우 발생합니다. </exception>
		template< class T >
		inline RefPtr<remove_refptr_t<T>> ResolveAs()
		{
			return Resolve().As<T>();
		}

		/// <summary> 개체의 참조 포인터를 가져옵니다. 실패할 경우 null을 반환합니다. </summary>
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

		/// <summary> (Visual Studio 전용) 개체의 존재 여부를 나타내는 값을 가져옵니다. </summary>
		vs_property_get( bool, IsValid );

		/// <summary> 개체의 존재 여부를 나타내는 값을 가져옵니다. </summary>
		bool IsValid_get();

		WeakPtr& operator=( const WeakPtr& copy );

	private:
		void AddRef();
		void Release();
		object Resolve();
	};
}