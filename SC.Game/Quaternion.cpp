using namespace SC;
using namespace SC::Game;

using namespace std;

const Quaternion Quaternion::Identity( Vector3::Zero, 1 );

Quaternion Quaternion::Slerp( const Quaternion& value, double t ) const
{
    return Slerp( value, t, 0 );
}

Quaternion Quaternion::Slerp( const Quaternion& value, double t, double minAngle, double maxAngle ) const
{
    static constexpr const double Threshold = 0.9995;

    auto v0 = Normalized;
    auto v1 = value.Normalized;

    double dot = v0.Dot( v1 );

    if ( dot < 0 )
    {
        v1 = -v1;
        dot = -dot;
    }

    double theta_0 = acos( dot );
    double theta = theta_0 * t;
    theta = clamp( theta, minAngle, maxAngle );
    if ( theta > theta_0 ) return value;

    if ( dot > Threshold )
    {
        return v0.Lerp( v1, t ).Normalized;
    }

    double sin_theta = sin( theta );
    double sin_theta_0 = sin( theta_0 );

    double s0 = cos( theta ) - dot * sin_theta / sin_theta_0;
    double s1 = sin_theta / sin_theta_0;

    return v0 * s0 + v1 * s1;
}

void Quaternion::V_set( Vector3 value )
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
}

Quaternion Quaternion::AngleAxis( double angle, Vector3 axis )
{
	auto sinv = sin( angle * 0.5 );
	auto cosv = cos( angle * 0.5 );
	return Quaternion(
		axis.Normalized * Vector3::Splat( sinv ),
		cosv
	);
}

Quaternion Quaternion::LookTo( Vector3 forward, Vector3 up )
{
    Vector3 vector = forward.Normalized;
    Vector3 vector2 = up.Cross( vector ).Normalized;
    Vector3 vector3 = vector.Cross( vector2 );

    auto m00 = vector2.X;
    auto m01 = vector2.Y;
    auto m02 = vector2.Z;
    auto m10 = vector3.X;
    auto m11 = vector3.Y;
    auto m12 = vector3.Z;
    auto m20 = vector.X;
    auto m21 = vector.Y;
    auto m22 = vector.Z;


    auto num8 = ( m00 + m11 ) + m22;
    auto quaternion = Quaternion();
    if ( num8 > 0 )
    {
        auto num = sqrt( num8 + 1 );
        quaternion.W = num * 0.5;
        num = 0.5 / num;
        quaternion.X = ( m12 - m21 ) * num;
        quaternion.Y = ( m20 - m02 ) * num;
        quaternion.Z = ( m01 - m10 ) * num;
        return quaternion;
    }
    if ( ( m00 >= m11 ) && ( m00 >= m22 ) )
    {
        auto num7 = sqrt( ( ( 1 + m00 ) - m11 ) - m22 );
        auto num4 = 0.5 / num7;
        quaternion.X = 0.5 * num7;
        quaternion.Y = ( m01 + m10 ) * num4;
        quaternion.Z = ( m02 + m20 ) * num4;
        quaternion.W = ( m12 - m21 ) * num4;
        return quaternion;
    }
    if ( m11 > m22 )
    {
        auto num6 = sqrt( ( ( 1 + m11 ) - m00 ) - m22 );
        auto num3 = 0.5f / num6;
        quaternion.X = ( m10 + m01 ) * num3;
        quaternion.Y = 0.5f * num6;
        quaternion.Z = ( m21 + m12 ) * num3;
        quaternion.W = ( m20 - m02 ) * num3;
        return quaternion;
    }
    auto num5 = sqrt( ( ( 1 + m22 ) - m00 ) - m11 );
    auto num2 = 0.5 / num5;
    quaternion.X = ( m20 + m02 ) * num2;
    quaternion.Y = ( m21 + m12 ) * num2;
    quaternion.Z = 0.5f * num5;
    quaternion.W = ( m01 - m10 ) * num2;
    return quaternion;
}

Quaternion Quaternion::FromToRotation( Vector3 from, Vector3 to )
{
    from = from.Normalized;
    to = to.Normalized;

    auto axis = from.Cross( to );
    auto angle = from.Dot( to );
    angle = acos( angle );

    // is nan
    if ( angle != angle ) return Quaternion::Identity;

    if ( angle < 0.001 ) return Quaternion::Identity;
    else if ( angle > 3.141 )
    {
        if ( from.X < from.Y && from.X < from.Z )
        {
            axis = Vector3( 0, 1, 0 ).Cross( from );
        }
        else if ( from.Y < from.X && from.Y < from.Z )
        {
            axis = Vector3( 0, 0, 1 ).Cross( from );
        }
        else
        {
            axis = Vector3( 1, 0, 0 ).Cross( from );
        }
    }

    return Quaternion::AngleAxis( angle, axis );
}