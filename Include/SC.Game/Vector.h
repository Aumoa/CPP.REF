#pragma once

namespace SC::Game
{
#pragma pack( push, 4 )
	/// <summary> 3차원 실수 형식을 사용하여 데이터를 표현합니다. </summary>
	struct __declspec( novtable ) Vector3
	{
		/// <summary> X축 값을 나타냅니다. </summary>
		double X;

		/// <summary> Y축 값을 나타냅니다. </summary>
		double Y;

		/// <summary> Z축 값을 나타냅니다. </summary>
		double Z;

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector3()
			: X( 0 )
			, Y( 0 )
			, Z( 0 )
		{

		}

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector3( double splat )
			: X( splat )
			, Y( splat )
			, Z( splat )
		{

		}

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector3( double x, double y, double z )
			: X( x )
			, Y( y )
			, Z( z )
		{

		}

		/// <summary> 대상 벡터와 내적 연산 결과를 가져옵니다. </summary>
		constexpr double Dot( const Vector3& value ) const
		{
			return X * value.X + Y * value.Y + Z * value.Z;
		}

		/// <summary> 대상 벡터와 외적 연산 결과를 가져옵니다. </summary>
		constexpr Vector3 Cross( const Vector3& value ) const
		{
			return Vector3( Y * value.Z - Z * value.Y, Z * value.X - X * value.Z, X * value.Y - Y * value.X );
		}

		/// <summary> 대상 벡터와 보간한 값을 가져옵니다. </summary>
		/// <param name="value"> 대상 벡터를 전달합니다. </param>
		/// <param name="t"> 대상 벡터의 비중을 전달합니다. </param>
		constexpr Vector3 Lerp( const Vector3& value, double t ) const
		{
			return ( *this * Splat( 1.0 - t ) + value * Splat( t ) );
		}

		/// <summary> (Visual Studio 전용) 벡터의 제곱한 크기를 가져옵니다. </summary>
		vs_property_get( double, MagnitudeSq );

		/// <summary> 벡터의 제곱한 크기를 가져옵니다. </summary>
		constexpr double MagnitudeSq_get() const
		{
			return X * X + Y * Y + Z * Z;
		}

		/// <summary> (Visual Studio 전용) 벡터의 크기를 가져옵니다. </summary>
		vs_property_get( double, Magnitude );

		/// <summary> 벡터의 크기를 가져옵니다. </summary>
		double Magnitude_get() const
		{
			return std::sqrt( MagnitudeSq_get() );
		}

		/// <summary> (Visual Studio 전용) 정규화된 벡터를 가져옵니다. </summary>
		vs_property_get( Vector3, Normalized );

		/// <summary> 정규화된 벡터를 가져옵니다. </summary>
		Vector3 Normalized_get() const;
		
		constexpr Vector3 operator-() const
		{
			return Vector3( -X, -Y, -Z );
		}

		constexpr Vector3 operator+( const Vector3& right ) const
		{
			return Vector3( X + right.X, Y + right.Y, Z + right.Z );
		}

		constexpr Vector3 operator-( const Vector3& right ) const
		{
			return Vector3( X - right.X, Y - right.Y, Z - right.Z );
		}

		constexpr Vector3 operator*( const Vector3& right ) const
		{
			return Vector3( X * right.X, Y * right.Y, Z * right.Z );
		}

		constexpr Vector3 operator/( const Vector3& right ) const
		{
			return Vector3( X / right.X, Y / right.Y, Z / right.Z );
		}

		Vector3 operator%( const Vector3& right ) const;
		Vector3& operator+=( const Vector3& right );
		Vector3& operator-=( const Vector3& right );
		Vector3& operator*=( const Vector3& right );
		Vector3& operator/=( const Vector3& right );
		Vector3& operator%=( const Vector3& right );

		constexpr bool operator==( const Vector3& right ) const
		{
			return X == right.X && Y == right.Y && Z == right.Z;
		}

		constexpr bool operator!=( const Vector3& right ) const
		{
			return X != right.X || Y != right.Y || Z != right.Z;
		}

		/// <summary> 한 개의 값으로 통일된 벡터를 생성합니다. </summary>
		static constexpr const Vector3 Splat( double value )
		{
			return Vector3( value, value, value );
		}

		/// <summary> 전방을 향하는 벡터를 가져옵니다. </summary>
		static const Vector3 Forward;

		/// <summary> 우측을 향하는 벡터를 가져옵니다. </summary>
		static const Vector3 Right;

		/// <summary> 위를 향하는 벡터를 가져옵니다. </summary>
		static const Vector3 Up;

		/// <summary> 값이 0으로 채워진 벡터를 가져옵니다. </summary>
		static const Vector3 Zero;

		/// <summary> 값이 1로 채워진 벡터를 가져옵니다. </summary>
		static const Vector3 One;
	};

	/// <summary> 2차원 실수 형식을 사용하여 데이터를 표현합니다. </summary>
	struct __declspec( novtable ) Vector2
	{
		/// <summary> X축 값을 나타냅니다. </summary>
		double X;

		/// <summary> Y축 값을 나타냅니다. </summary>
		double Y;

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector2()
			: X( 0 )
			, Y( 0 )
		{

		}

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector2( double splat )
			: X( splat )
			, Y( splat )
		{

		}

		/// <summary> <see cref="Vector3"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Vector2( double x, double y )
			: X( x )
			, Y( y )
		{

		}

		/// <summary> 대상 벡터와 내적 연산 결과를 가져옵니다. </summary>
		constexpr double Dot( const Vector2& value ) const
		{
			return X * value.X + Y * value.Y;
		}

		/// <summary> 대상 벡터와 보간한 값을 가져옵니다. </summary>
		/// <param name="value"> 대상 벡터를 전달합니다. </param>
		/// <param name="t"> 대상 벡터의 비중을 전달합니다. </param>
		constexpr Vector2 Lerp( const Vector2& value, double t ) const
		{
			return ( *this * Splat( 1.0 - t ) + value * Splat( t ) );
		}

		/// <summary> (Visual Studio 전용) 벡터의 제곱한 크기를 가져옵니다. </summary>
		vs_property_get( double, MagnitudeSq );

		/// <summary> 벡터의 제곱한 크기를 가져옵니다. </summary>
		constexpr double MagnitudeSq_get() const
		{
			return X * X + Y * Y;
		}

		/// <summary> (Visual Studio 전용) 벡터의 크기를 가져옵니다. </summary>
		vs_property_get( double, Magnitude );

		/// <summary> 벡터의 크기를 가져옵니다. </summary>
		double Magnitude_get() const
		{
			return std::sqrt( MagnitudeSq_get() );
		}

		/// <summary> (Visual Studio 전용) 정규화된 벡터를 가져옵니다. </summary>
		vs_property_get( Vector2, Normalized );

		/// <summary> 정규화된 벡터를 가져옵니다. </summary>
		Vector2 Normalized_get() const;

		constexpr Vector2 operator-() const
		{
			return Vector2( -X, -Y );
		}

		constexpr Vector2 operator+( const Vector2& right ) const
		{
			return Vector2( X + right.X, Y + right.Y );
		}

		constexpr Vector2 operator-( const Vector2& right ) const
		{
			return Vector2( X - right.X, Y - right.Y );
		}

		constexpr Vector2 operator*( const Vector2& right ) const
		{
			return Vector2( X * right.X, Y * right.Y );
		}

		constexpr Vector2 operator/( const Vector2& right ) const
		{
			return Vector2( X / right.X, Y / right.Y );
		}

		Vector2 operator%( const Vector2& right ) const;
		Vector2& operator+=( const Vector2& right );
		Vector2& operator-=( const Vector2& right );
		Vector2& operator*=( const Vector2& right );
		Vector2& operator/=( const Vector2& right );
		Vector2& operator%=( const Vector2& right );

		constexpr bool operator==( const Vector2& right ) const
		{
			return X == right.X && Y == right.Y;
		}

		constexpr bool operator!=( const Vector2& right ) const
		{
			return X != right.X || Y != right.Y;
		}

		/// <summary> 한 개의 값으로 통일된 벡터를 생성합니다. </summary>
		static constexpr const Vector2 Splat( double value )
		{
			return Vector2( value, value );
		}

		/// <summary> 값이 0으로 채워진 벡터를 가져옵니다. </summary>
		static const Vector2 Zero;

		/// <summary> 값이 1로 채워진 벡터를 가져옵니다. </summary>
		static const Vector2 One;
	};
#pragma pack( pop )
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::Vector3 value )
{
	return wout << L"(" << value.X << L", " << value.Y << L", " << value.Z << L")";
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::Vector2 value )
{
	return wout << L"(" << value.X << L", " << value.Y << L")";
}