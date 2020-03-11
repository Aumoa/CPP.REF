#pragma once

namespace SC
{
#pragma pack( push, 1 )
	/// <summary> 숫자 형식 구조체에 대한 기본 함수를 제공합니다. </summary>
	template< class T >
	struct NumericType : public ValueType, virtual public IEquatable<T>, virtual public IComparable<T>
	{
		/// <summary> 값을 설정합니다. </summary>
		T Value;

		/// <summary> <see cref="NumericType"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline NumericType( T value = T( 0 ) );

		/// <summary> <see cref="NumericType"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		inline NumericType( const NumericType& copy );

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 현재 개체의 텍스트 표현을 생성하여 반환합니다. </summary>
		inline String ToString() override;

		/// <summary> (<see cref="Object"/> 클래스에서 상속 됨.) 지정한 개체가 현재 개체와 같은지 확인합니다. </summary>
		/// <param name="obj"> 비교할 개체를 전달합니다. </param>
		inline bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체가 동일한 종류의 다른 개체와 동일한지 비교합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		inline bool Equals( T right ) override;

		/// <summary> (<see cref="IEquatable"/> 인터페이스에서 구현 됨.) 현재 개체와 동일한 형식의 다른 개체를 비교하고 정렬 순서에서 현재 개체의 위치가 다른 개체보다 앞인지, 뒤인지 또는 동일한지를 나타내는 정수를 반환합니다. </summary>
		/// <param name="right"> 동일한 종류의 개체를 전달합니다. </param>
		inline int CompareTo( T right ) override;

		inline T* operator&();
		inline const T* operator&() const;
		inline operator T&();
		inline operator const T&() const;

		/// <summary> 숫자의 문자열 표현을 해당하는 자료형으로 변환합니다. </summary>
		/// <param name="text"> 숫자의 문자열 표현을 전달합니다. </param>
		static inline T Parse( String text );

		/// <summary> 숫자의 문자열 표현을 해당하는 자료형으로 변환합니다. 반환값은 작업 성공 여부를 나타냅니다. </summary>
		/// <param name="text"> 숫자의 문자열 표현을 전달합니다. </param>
		/// <param name="pOut"> 작업 결과를 받을 주소를 전달합니다. </param>
		static inline bool TryParse( String text, T* pOut );
	};

	using Int8 = NumericType<__int8>;
	using UInt8 = NumericType<unsigned __int8>;
	using Int16 = NumericType<__int16>;
	using UInt16 = NumericType<unsigned __int16>;
	using Int32 = NumericType<__int32>;
	using UInt32 = NumericType<unsigned __int32>;
	using Int64 = NumericType<__int64>;
	using UInt64 = NumericType<unsigned __int64>;
	using Single = NumericType<float>;
	using Double = NumericType<double>;
#pragma pack( pop )
}