#pragma once

namespace SC
{
#pragma pack( push, 1 )
	/// <summary> ���� ���� ����ü�� ���� �⺻ �Լ��� �����մϴ�. </summary>
	template< class T >
	struct NumericType : public ValueType, virtual public IEquatable<T>, virtual public IComparable<T>
	{
		/// <summary> ���� �����մϴ�. </summary>
		T Value;

		/// <summary> <see cref="NumericType"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline NumericType( T value = T( 0 ) );

		/// <summary> <see cref="NumericType"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		inline NumericType( const NumericType& copy );

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ���� ��ü�� �ؽ�Ʈ ǥ���� �����Ͽ� ��ȯ�մϴ�. </summary>
		inline String ToString() override;

		/// <summary> (<see cref="Object"/> Ŭ�������� ��� ��.) ������ ��ü�� ���� ��ü�� ������ Ȯ���մϴ�. </summary>
		/// <param name="obj"> ���� ��ü�� �����մϴ�. </param>
		inline bool Equals( object obj ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� �������� ���մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		inline bool Equals( T right ) override;

		/// <summary> (<see cref="IEquatable"/> �������̽����� ���� ��.) ���� ��ü�� ������ ������ �ٸ� ��ü�� ���ϰ� ���� �������� ���� ��ü�� ��ġ�� �ٸ� ��ü���� ������, ������ �Ǵ� ���������� ��Ÿ���� ������ ��ȯ�մϴ�. </summary>
		/// <param name="right"> ������ ������ ��ü�� �����մϴ�. </param>
		inline int CompareTo( T right ) override;

		inline T* operator&();
		inline const T* operator&() const;
		inline operator T&();
		inline operator const T&() const;

		/// <summary> ������ ���ڿ� ǥ���� �ش��ϴ� �ڷ������� ��ȯ�մϴ�. </summary>
		/// <param name="text"> ������ ���ڿ� ǥ���� �����մϴ�. </param>
		static inline T Parse( String text );

		/// <summary> ������ ���ڿ� ǥ���� �ش��ϴ� �ڷ������� ��ȯ�մϴ�. ��ȯ���� �۾� ���� ���θ� ��Ÿ���ϴ�. </summary>
		/// <param name="text"> ������ ���ڿ� ǥ���� �����մϴ�. </param>
		/// <param name="pOut"> �۾� ����� ���� �ּҸ� �����մϴ�. </param>
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