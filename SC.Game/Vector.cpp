using namespace SC;
using namespace SC::Game;

using namespace std;

const Vector3 Vector3::Forward( 0, 0, 1 );
const Vector3 Vector3::Right( 1, 0, 0 );
const Vector3 Vector3::Up( 0, 1, 0 );
const Vector3 Vector3::Zero( 0, 0, 0 );
const Vector3 Vector3::One( 1, 1, 1 );

Vector3 Vector3::Normalized_get() const
{
	return ( *this ) / Splat( Magnitude );
}

Vector3 Vector3::operator%( const Vector3& right ) const
{
	return Vector3( fmod( X, right.X ), fmod( Y, right.Y ), fmod( Z, right.Z ) );
}

Vector3& Vector3::operator+=( const Vector3& right )
{
	X += right.X;
	Y += right.Y;
	Z += right.Z;
	return *this;
}

Vector3& Vector3::operator-=( const Vector3& right )
{
	X -= right.X;
	Y -= right.Y;
	Z -= right.Z;
	return *this;
}

Vector3& Vector3::operator*=( const Vector3& right )
{
	X *= right.X;
	Y *= right.Y;
	Z *= right.Z;
	return *this;
}

Vector3& Vector3::operator/=( const Vector3& right )
{
	X /= right.X;
	Y /= right.Y;
	Z /= right.Z;
	return *this;
}

Vector3& Vector3::operator%=( const Vector3& right )
{
	X = fmod( X, right.X );
	Y = fmod( Y, right.Y );
	Z = fmod( Z, right.Z );
	return *this;
}

const Vector2 Vector2::Zero( 0, 0 );
const Vector2 Vector2::One( 1, 1 );

Vector2 Vector2::Normalized_get() const
{
	return ( *this ) / Splat( Magnitude );
}

Vector2 Vector2::operator%( const Vector2& right ) const
{
	return Vector2( fmod( X, right.X ), fmod( Y, right.Y ) );
}

Vector2& Vector2::operator+=( const Vector2& right )
{
	X += right.X;
	Y += right.Y;
	return *this;
}

Vector2& Vector2::operator-=( const Vector2& right )
{
	X -= right.X;
	Y -= right.Y;
	return *this;
}

Vector2& Vector2::operator*=( const Vector2& right )
{
	X *= right.X;
	Y *= right.Y;
	return *this;
}

Vector2& Vector2::operator/=( const Vector2& right )
{
	X /= right.X;
	Y /= right.Y;
	return *this;
}

Vector2& Vector2::operator%=( const Vector2& right )
{
	X = fmod( X, right.X );
	Y = fmod( Y, right.Y );
	return *this;
}