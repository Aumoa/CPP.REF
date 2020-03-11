#pragma once

namespace SC
{
	/// <summary> <see cref="NumericType"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline NumericType<T>::NumericType( T value )
		: Value( value )
	{

	}

	/// <summary> <see cref="NumericType"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
	template< class T >
	inline NumericType<T>::NumericType( const NumericType& copy )
		: Value( copy.Value )
	{

	}

	/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
	template< class T >
	inline String NumericType<T>::ToString()
	{
		std::wostringstream woss;
		woss << Value;
		return woss.str().c_str();
	}

	/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
	/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
	template< class T >
	inline bool NumericType<T>::Equals( object obj )
	{
		if ( T right1; obj.Is<T>( &right1 ) )
		{
			return Value == right1;
		}
		//else if ( NumericType right2; obj.Is<NumericType>( &right2 ) )
		//{
		//	return Value == right2.Value;
		//}
		else
		{
			return false;
		}
	}

	/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
	/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
	template< class T >
	inline bool NumericType<T>::Equals( T right )
	{
		return Value == right;
	}

	/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
	/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
	template< class T >
	inline int NumericType<T>::CompareTo( T right )
	{
		if ( Value < right ) return -1;
		else if ( Value > right ) return 1;
		else return 0;
	}

	template< class T >
	inline T* NumericType<T>::operator&()
	{
		return &Value;
	}

	template< class T >
	inline const T* NumericType<T>::operator&() const
	{
		return &Value;
	}

	template< class T >
	inline NumericType<T>::operator T&()
	{
		return Value;
	}

	template< class T >
	inline NumericType<T>::operator const T&() const
	{
		return Value;
	}

	/// <summary> ������ ���ڿ� ǥ���� �ش��ϴ� �ڷ������� ��ȯ�մϴ�. </summary>
	/// <param name="text"> ������ ���ڿ� ǥ���� �����մϴ�. </param>
	template< class T >
	inline T NumericType<T>::Parse( String text )
	{
		T value;
		std::wistringstream wiss( text.Chars_get() );
		wiss >> value;
		return value;
	}

	/// <summary> ������ ���ڿ� ǥ���� �ش��ϴ� �ڷ������� ��ȯ�մϴ�. ��ȯ���� �۾� ���� ���θ� ��Ÿ���ϴ�. </summary>
	/// <param name="text"> ������ ���ڿ� ǥ���� �����մϴ�. </param>
	/// <param name="pOut"> �۾� ����� ���� �ּҸ� �����մϴ�. </param>
	template< class T >
	inline bool NumericType<T>::TryParse( String text, T* pOut )
	{
		if ( pOut ) *pOut = Parse( text );
		return true;
	}
}