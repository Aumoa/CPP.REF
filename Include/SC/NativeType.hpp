#pragma once

namespace SC
{
	/// <summary> <see cref="NativeType"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
	/// <param name="value"> 지정된 형식의 네이티브 값을 전달합니다. </param>
	template< class T >
	template< typename std::enable_if_t<!where<Object, T>>* >
	inline NativeType<T>::NativeType( const T& value )
	{
		ptr = new T( value );
	}

	template< class T >
	inline NativeType<T>::~NativeType()
	{
		if ( ptr )
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
	template< class T >
	inline String NativeType<T>::ToString()
	{
		return ToStringT<T>();
	}

	/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
	/// <param name="obj"> 비교할 개체를 전달합니다. </param>
	template< class T >
	inline bool NativeType<T>::Equals( object obj )
	{
		if ( T value; obj.Is<T>( &value ) )
		{
			return *ptr == value;
		}
		else
		{
			return false;
		}
	}

	/// <summary> 네이티브 값 형식을 언박싱합니다. </summary>
	template< class T >
	inline T NativeType<T>::Unboxing()
	{
		return *ptr;
	}
}