#pragma once

namespace SC
{
	/// <summary> <see cref="NumericType"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
	template< class T >
	inline NumericType<T>::NumericType( T value )
		: Value( value )
	{

	}

	/// <summary> <see cref="NumericType"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
	template< class T >
	inline NumericType<T>::NumericType( const NumericType& copy )
		: Value( copy.Value )
	{

	}

	/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
	template< class T >
	inline String NumericType<T>::ToString()
	{
		std::wostringstream woss;
		woss << Value;
		return woss.str().c_str();
	}

	/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
	/// <param name="obj"> 비교할 개체를 전달합니다. </param>
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

	/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
	/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
	template< class T >
	inline bool NumericType<T>::Equals( T right )
	{
		return Value == right;
	}

	/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
	/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
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

	/// <summary> 숫자의 문자열 표현을 해당하는 자료형으로 변환합니다. </summary>
	/// <param name="text"> 숫자의 문자열 표현을 전달합니다. </param>
	template< class T >
	inline T NumericType<T>::Parse( String text )
	{
		T value;
		std::wistringstream wiss( text.Chars_get() );
		wiss >> value;
		return value;
	}

	/// <summary> 숫자의 문자열 표현을 해당하는 자료형으로 변환합니다. 반환값은 작업 성공 여부를 나타냅니다. </summary>
	/// <param name="text"> 숫자의 문자열 표현을 전달합니다. </param>
	/// <param name="pOut"> 작업 결과를 받을 주소를 전달합니다. </param>
	template< class T >
	inline bool NumericType<T>::TryParse( String text, T* pOut )
	{
		if ( pOut ) *pOut = Parse( text );
		return true;
	}
}