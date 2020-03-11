using namespace SC;
using namespace SC::Game;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

using namespace std;

RevealBrush::RevealBrush( Drawing::Color color ) : Brush( 1 )
{
	frameResource->Attribute0 = { color.R, color.G, color.B, color.A };
	frameResource->Attribute1.x = 100.0f;
	frameResource->Attribute1.y = 0;
}

Drawing::Color RevealBrush::RevealColor_get()
{
	return Drawing::Color( frameResource->Attribute0.x, frameResource->Attribute0.y, frameResource->Attribute0.z, frameResource->Attribute0.w );
}

void RevealBrush::RevealColor_set( Drawing::Color value )
{
	frameResource->Attribute0 = { value.R, value.G, value.B, value.A };
}

double RevealBrush::RevealDistance_get()
{
	return ( double )frameResource->Attribute1.x;
}

void RevealBrush::RevealDistance_set( double value )
{
	frameResource->Attribute1.x = ( float )value;
}

double RevealBrush::MinOpacity_get()
{
	return ( double )frameResource->Attribute1.y;
}

void RevealBrush::MinOpacity_set( double value )
{
	frameResource->Attribute1.y = ( float )value;
}