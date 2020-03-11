#pragma once

namespace SC::Game
{

#pragma pack( push, 4 )
	/// <summary> 4���� ���� ǥ���ϴ� ������� �����մϴ�. </summary>
	struct Quaternion
	{
		/// <summary> ��� ��Ʈ X�� ���� ��Ÿ���ϴ�. </summary>
		double X;

		/// <summary> ��� ��Ʈ Y�� ���� ��Ÿ���ϴ�. </summary>
		double Y;

		/// <summary> ��� ��Ʈ Z�� ���� ��Ÿ���ϴ�. </summary>
		double Z;

		/// <summary> �Ǽ� ��Ʈ ���� ��Ÿ���ϴ�. </summary>
		double W;

		/// <summary> <see cref="Quaternion"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		constexpr Quaternion( double x = 0, double y = 0, double z = 0, double w = 1 )
			: X( x )
			, Y( y )
			, Z( z )
			, W( w )
		{

		}

		/// <summary> <see cref="Quaternion"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		/// <param name="v"> ����� ���͸� �����մϴ�. </param>
		/// <param name="w"> �Ǽ��� ���͸� �����մϴ�. </param>
		constexpr Quaternion( Vector3 v, double w = 1 )
			: X( v.X )
			, Y( v.Y )
			, Z( v.Z )
			, W( w )
		{

		}

		/// <summary> ��� ������� ���� ���� ����� �����ɴϴ�. </summary>
		constexpr double Dot( const Quaternion& value ) const
		{
			return X * value.X + Y * value.Y + Z * value.Z + W * value.W;
		}

		/// <summary> ��� ������� ������ ���� �����ɴϴ�. </summary>
		constexpr Quaternion Lerp( const Quaternion& value, double t ) const
		{
			const double i = 1 - t;
			return Quaternion( X * i + value.X * t, Y * i + value.Y * t, Z * i + value.Z * t, W * i + value.W * t );
		}

		/// <summary> ��� ������� ���� ���� ������ ���� �����ɴϴ�. </summary>
		/// <param name="value"> ��� ������� �����մϴ�. </param>
		/// <param name="t"> ���� �Ű����� T�� �����մϴ�. </param>
		Quaternion Slerp( const Quaternion& value, double t ) const;

		/// <summary> ��� ������� ���� ���� ������ ���� �����ɴϴ�. </summary>
		/// <param name="value"> ��� ������� �����մϴ�. </param>
		/// <param name="t"> ���� �Ű����� T�� �����մϴ�. </param>
		/// <param name="minAngle"> �ּ� ȸ�� ������ �����մϴ�. </param>
		/// <param name="maxAngle"> �ִ� ȸ�� ������ �����մϴ�. </param>
		Quaternion Slerp( const Quaternion& value, double t, double minAngle, double maxAngle = 3.141592 ) const;

		/// <summary> (Visual Studio ����) ����ȭ�� ������� �����ɴϴ�. </summary>
		vs_property_get( Quaternion, Normalized );

		/// <summary> ����ȭ�� ������� �����ɴϴ�. </summary>
		Quaternion Normalized_get() const
		{
			auto length = X * X + Y * Y + Z * Z + W * W;
			return Quaternion( X / length, Y / length, Z / length, W / length );
		}

		/// <summary> (Visual Studio ����) �ӷ� ������� �����ɴϴ�. </summary>
		vs_property_get( Quaternion, Conjugate );

		/// <summary> �ӷ� ������� �����ɴϴ�. </summary>
		constexpr Quaternion Conjugate_get() const
		{
			return Quaternion( -X, -Y, -Z, W );
		}

		/// <summary> (Visual Studio ����) ������� ��� �κ��� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( Vector3, V );

		/// <summary> ������� ��� �κ��� ���� �����ɴϴ�. </summary>
		constexpr Vector3 V_get() const
		{
			return Vector3( X, Y, Z );
		}

		/// <summary> ������� ��� �κ��� ���� �����մϴ�. </summary>
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

		/// <summary> �࿡ ���� ȸ���� ǥ���ϴ� ������� �����ɴϴ�. </summary>
		/// <param name="angle"> ȸ�� ������ ���� ������ �����մϴ�. </param>
		/// <param name="axis"> ȸ�� ���� �����մϴ�. </param>
		static Quaternion AngleAxis( double angle, Vector3 axis );

		/// <summary> ������ �ٶ󺸴� ȸ�� ������� �����ɴϴ�. </summary>
		/// <param name="forward"> ������ �����մϴ�. </param>
		/// <param name="up"> ���� ���͸� �����մϴ�. </param>
		static Quaternion LookTo( Vector3 forward, Vector3 up );

		/// <summary> ���� ���⿡�� �� �������� ��ȯ�ϴ� ȸ�� ������� �����ɴϴ�. </summary>
		/// <param name="from"> ���� ���� ���͸� �����մϴ�. </param>
		/// <param name="to"> �� ���� ���͸� �����մϴ�. </param>
		static Quaternion FromToRotation( Vector3 from, Vector3 to );

		/// <summary> ���� ������� �����ɴϴ�. </summary>
		static const Quaternion Identity;
	};
#pragma pack( pop )
}

inline std::wostream& operator<<( std::wostream& wout, SC::Game::Quaternion value )
{
	return wout << L"{(" << value.X << L", " << value.Y << L", " << value.Z << L"), " << value.W << L"}";
}