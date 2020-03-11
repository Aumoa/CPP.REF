#pragma once

namespace SC
{
	/// <summary> <see cref="NativeType"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	/// <param name="value"> ������ ������ ����Ƽ�� ���� �����մϴ�. </param>
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

	/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
	template< class T >
	inline String NativeType<T>::ToString()
	{
		return ToStringT<T>();
	}

	/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
	/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
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

	/// <summary> ����Ƽ�� �� ������ ��ڽ��մϴ�. </summary>
	template< class T >
	inline T NativeType<T>::Unboxing()
	{
		return *ptr;
	}
}