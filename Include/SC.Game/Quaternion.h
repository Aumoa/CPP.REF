#pragma once

namespace SC::Game
{

#pragma pack( push, 4 )
	/// <summary> 4차원 수를 표현하는 사원수를 정의합니다. </summary>
	struct Quaternion
	{
		/// <summary> 허수 파트 X축 값을 나타냅니다. </summary>
		double X;

		/// <summary> 허수 파트 Y축 값을 나타냅니다. </summary>
		double Y;

		/// <summary> 허수 파트 Z축 값을 나타냅니다. </summary>
		double Z;

		/// <summary> 실수 파트 값을 나타냅니다. </summary>
		double W;

		/// <summary> <see cref="Quaternion"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		constexpr Quaternion( double x = 0, double y = 0, double z = 0, double w = 1 )
			: X( x )
			, Y( y )
			, Z( z )
			, W( w )
		{

		}

		/// <summary> <see cref="Quaternion"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		/// <param name="v"> 허수부 벡터를 전달합니다. </param>
		/// <param name="w"> 실수부 벡터를 전달합니다. </param>
		constexpr Quaternion( Vector3 v, double w = 1 )
			: X( v.X )
			, Y( v.Y )
			, Z( v.Z )
			, W( w )
		{

		}

		/// <summary> 대상 사원수와 내적 연산 결과를 가져옵니다. </summary>
		constexpr double Dot( const Quaternion& value ) const
		{
			return X * value.X + Y * value.Y + Z * value.Z + W * value.W;
		}

		/// <summary> 대상 사원수와 보간한 값을 가져옵니다. </summary>
		constexpr Quaternion Lerp( const Quaternion& value, double t ) const
		{
			const double i = 1 - t;
			return Quaternion( X * i + value.X * t, Y * i + value.Y * t, Z * i + value.Z * t, W * i + value.W * t );
		}

		/// <summary> 대상 사원수와 구면 선형 보간한 값을 가져옵니다. </summary>
		/// <param name="value"> 대상 사원수를 전달합니다. </param>
		/// <param name="t"> 보간 매개변수 T를 전달합니다. </param>
		Quaternion Slerp( const Quaternion& value, double t ) const;

		/// <summary> 대상 사원수와 구면 선형 보간한 값을 가져옵니다. </summary>
		/// <param name="value"> 대상 사원수를 전달합니다. </param>
		/// <param name="t"> 보간 매개변수 T를 전달합니다. </param>
		/// <param name="minAngle"> 최소 회전 각도를 전달합니다. </param>
		/// <param name="maxAngle"> 최대 회전 각도를 전달합니다. </param>
		Quaternion Slerp( const Quaternion& value, double t, double minAngle, double maxAngle = 3.141592 ) const;

		/// <summary> (Visual Studio 전용) 정규화된 사원수를 가져옵니다. </summary>
		vs_property_get( Quaternion, Normalized );

		/// <summary> 정규화된 사원수를 가져옵니다. </summary>
		Quaternion Normalized_get() const
		{
			auto length = X * X + Y * Y + Z * Z + W * W;
			return Quaternion( X / length, Y / length, Z / length, W / length );
		}

		/// <summary> (Visual Studio 전용) 켤레 사원수를 가져옵니다. </summary>
		vs_property_get( Quaternion, Conjugate );

		/// <summary> 켤레 사원수를 가져옵니다. </summary>
		constexpr Quaternion Conjugate_get() const
		{
			return Quaternion( -X, -Y, -Z, W );
		}

		/// <summary> (Visual Studio 전용) 사원수의 허수 부분의 값을 설정하거나 가져옵니다. </summary>
		vs_property( Vector3, V );

		/// <summary> 사원수의 허수 부분의 값을 가져옵니다. </summary>
		constexpr Vector3 V_get() const
		{
			return Vector3( X, Y, Z );
		}

		/// <summary> 사원수의 허수 부분의 값을 설정합니다. </summary>
		void V_set( Vector3 value );

		constexpr Quaternion operator-() const
		{
			return Quaternion( -X, -Y, -Z, -W );
		}

		constexpr Quaternion operator+( const Quaternion& right ) const
		{
			return Quaternion( X + right.X, Y + right.Y, Z + right.Z, W + right.W );
		}

		constexpr Quaternion operator-( const Quaternion& right ) const
		{
			return Quaternion( X - right.X, Y - right.Y, Z - right.Z, W - right.W );
		}

		constexpr Quaternion operator*( const Quaternion& value ) const
		{
			return Quaternion( value.V * Vector3::Splat( W ) + V * Vector3::Splat( value.W ) + V.Cross( value.V ), W * value.W - V.Dot( value.V ) );
		}

		constexpr Quaternion operator*( double value ) const
		{
			return Quaternion( X * value, Y * value, Z * value, W * value );
		}

		constexpr Quaternion operator/( double value ) const
		{
			return Quaternion( X / value, Y / value, Z / value, W / value );
		}

		constexpr bool operator==( const Quaternion& right ) const
		{
			return X == right.X && Y == right.Y && Z == right.Z && W == right.W;
		}

		constexpr bool operator!=( const Quaternion& right ) const
		{
			return X != right.X || Y != right.Y || Z != right.Z || W != right.W;
		}

		/// <summary> 축에 의한 회전을 표현하는 사원수를 가져옵니다. </summary>
		/// <param name="angle"> 회전 각도를 라디안 단위로 전달합니다. </param>
		/// <param name="axis"> 회전 축을 전달합니다. </param>
		static Quaternion AngleAxis( double angle, Vector3 axis );

		/// <summary> 방향을 바라보는 회전 사원수를 가져옵니다. </summary>
		/// <param name="forward"> 방향을 전달합니다. </param>
		/// <param name="up"> 상향 벡터를 전달합니다. </param>
		static Quaternion LookTo( Vector3 forward, Vector3 up );

		/// <summary> 시작 방향에서 끝 방향으로 전환하는 회전 사원수를 가져옵니다. </summary>
		/// <param name="from"> 시작 방향 벡터를 전달합니다. </param>
		/// <param name="to"> 끝 방향 벡터를 전달합니다. </param>
		static Quaternion FromToRotation( Vector3 from, Vector3 to );

		/// <summary> 단위 사원수를 가져옵니다. </summary>
		static const Quaternion Identity;
	};
#pragma pack( pop )
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::Quaternion value )
{
	return wout << L"{(" << value.X << L", " << value.Y << L", " << value.Z << L"), " << value.W << L"}";
}