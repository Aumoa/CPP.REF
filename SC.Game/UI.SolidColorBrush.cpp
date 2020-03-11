using namespace SC;
using namespace SC::Game::UI;
using namespace SC::Game::Details;

using namespace std;

SolidColorBrush::SolidColorBrush( Drawing::Color color ) : Brush( 0 )
{
	frameResource->Attribute0 = { color.R, color.G, color.B, color.A };
}

Drawing::Color SolidColorBrush::SolidColor_get()
{
	return Drawing::Color( frameResource->Attribute0.x, frameResource->Attribute0.y, frameResource->Attribute0.z, frameResource->Attribute0.w );
}

void SolidColorBrush::SolidColor_set( Drawing::Color value )
{
	frameResource->Attribute0 = { value.R, value.G, value.B, value.A };
}