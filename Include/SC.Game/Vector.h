#pragma once

namespace SC::Game
{
#pragma pack( push, 4 )
	/// <summary> 3���� �Ǽ� ������ ����Ͽ� �����͸� ǥ���մϴ�. </summary>
	struct __declspec( novtable ) Vector3
	{
		/// <summary> X�� ���� ��Ÿ���ϴ�. </summary>
		double X;

		/// <summary> Y�� ���� ��Ÿ���ϴ�. </summary>
		double Y;

		/// <summary> Z�� ���� ��Ÿ���ϴ�. </summary>
		double Z;

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector3()
			: X( 0 )
			, Y( 0 )
			, Z( 0 )
		{

		}

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector3( double splat )
			: X( splat )
			, Y( splat )
			, Z( splat )
		{

		}

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector3( double x, double y, double z )
			: X( x )
			, Y( y )
			, Z( z )
		{

		}

		/// <summary> ��� ���Ϳ� ���� ���� ����� �����ɴϴ�. </summary>
		constexpr double Dot( const Vector3& value ) const
		{
			return X * value.X + Y * value.Y + Z * value.Z;
		}

		/// <summary> ��� ���Ϳ� ���� ���� ����� �����ɴϴ�. </summary>
		constexpr Vector3 Cross( const Vector3& value ) const
		{
			return Vector3( Y * value.Z - Z * value.Y, Z * value.X - X * value.Z, X * value.Y - Y * value.X );
		}

		/// <summary> ��� ���Ϳ� ������ ���� �����ɴϴ�. </summary>
		/// <param name="value"> ��� ���͸� �����մϴ�. </param>
		/// <param name="t"> ��� ������ ������ �����մϴ�. </param>
		constexpr Vector3 Lerp( const Vector3& value, double t ) const
		{
			return ( *this * Splat( 1.0 - t ) + value * Splat( t ) );
		}

		/// <summary> (Visual Studio ����) ������ ������ ũ�⸦ �����ɴϴ�. </summary>
		vs_property_get( double, MagnitudeSq );

		/// <summary> ������ ������ ũ�⸦ �����ɴϴ�. </summary>
		constexpr double MagnitudeSq_get() const
		{
			return X * X + Y * Y + Z * Z;
		}

		/// <summary> (Visual Studio ����) ������ ũ�⸦ �����ɴϴ�. </summary>
		vs_property_get( double, Magnitude );

		/// <summary> ������ ũ�⸦ �����ɴϴ�. </summary>
		double Magnitude_get() const
		{
			return std::sqrt( MagnitudeSq_get() );
		}

		/// <summary> (Visual Studio ����) ����ȭ�� ���͸� �����ɴϴ�. </summary>
		vs_property_get( Vector3, Normalized );

		/// <summary> ����ȭ�� ���͸� �����ɴϴ�. </summary>
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

		/// <summary> �� ���� ������ ���ϵ� ���͸� �����մϴ�. </summary>
		static constexpr const Vector3 Splat( double value )
		{
			return Vector3( value, value, value );
		}

		/// <summary> ������ ���ϴ� ���͸� �����ɴϴ�. </summary>
		static const Vector3 Forward;

		/// <summary> ������ ���ϴ� ���͸� �����ɴϴ�. </summary>
		static const Vector3 Right;

		/// <summary> ���� ���ϴ� ���͸� �����ɴϴ�. </summary>
		static const Vector3 Up;

		/// <summary> ���� 0���� ä���� ���͸� �����ɴϴ�. </summary>
		static const Vector3 Zero;

		/// <summary> ���� 1�� ä���� ���͸� �����ɴϴ�. </summary>
		static const Vector3 One;
	};

	/// <summary> 2���� �Ǽ� ������ ����Ͽ� �����͸� ǥ���մϴ�. </summary>
	struct __declspec( novtable ) Vector2
	{
		/// <summary> X�� ���� ��Ÿ���ϴ�. </summary>
		double X;

		/// <summary> Y�� ���� ��Ÿ���ϴ�. </summary>
		double Y;

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector2()
			: X( 0 )
			, Y( 0 )
		{

		}

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector2( double splat )
			: X( splat )
			, Y( splat )
		{

		}

		/// <summary> <see cref="Vector3"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Vector2( double x, double y )
			: X( x )
			, Y( y )
		{

		}

		/// <summary> ��� ���Ϳ� ���� ���� ����� �����ɴϴ�. </summary>
		constexpr double Dot( const Vector2& value ) const
		{
			return X * value.X + Y * value.Y;
		}

		/// <summary> ��� ���Ϳ� ������ ���� �����ɴϴ�. </summary>
		/// <param name="value"> ��� ���͸� �����մϴ�. </param>
		/// <param name="t"> ��� ������ ������ �����մϴ�. </param>
		constexpr Vector2 Lerp( const Vector2& value, double t ) const
		{
			return ( *this * Splat( 1.0 - t ) + value * Splat( t ) );
		}

		/// <summary> (Visual Studio ����) ������ ������ ũ�⸦ �����ɴϴ�. </summary>
		vs_property_get( double, MagnitudeSq );

		/// <summary> ������ ������ ũ�⸦ �����ɴϴ�. </summary>
		constexpr double MagnitudeSq_get() const
		{
			return X * X + Y * Y;
		}

		/// <summary> (Visual Studio ����) ������ ũ�⸦ �����ɴϴ�. </summary>
		vs_property_get( double, Magnitude );

		/// <summary> ������ ũ�⸦ �����ɴϴ�. </summary>
		double Magnitude_get() const
		{
			return std::sqrt( MagnitudeSq_get() );
		}

		/// <summary> (Visual Studio ����) ����ȭ�� ���͸� �����ɴϴ�. </summary>
		vs_property_get( Vector2, Normalized );

		/// <summary> ����ȭ�� ���͸� �����ɴϴ�. </summary>
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

		/// <summary> �� ���� ������ ���ϵ� ���͸� �����մϴ�. </summary>
		static constexpr const Vector2 Splat( double value )
		{
			return Vector2( value, value );
		}

		/// <summary> ���� 0���� ä���� ���͸� �����ɴϴ�. </summary>
		static const Vector2 Zero;

		/// <summary> ���� 1�� ä���� ���͸� �����ɴϴ�. </summary>
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