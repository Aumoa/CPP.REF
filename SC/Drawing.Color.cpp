using namespace SC;
using namespace SC::Drawing;

using namespace std;

const Color Color::AliceBlue			= 0xF0F8FF;
const Color Color::AntiqueWhite			= 0xFAEBD7;
const Color Color::Aqua					= 0x00FFFF;
const Color Color::Aquamarine			= 0x7FFFD4;
const Color Color::Azure				= 0xF0FFFF;
const Color Color::Beige				= 0xF5F5DC;
const Color Color::Bisque				= 0xFFE4C4;
const Color Color::Black				= 0x000000;
const Color Color::BlanchedAlmond		= 0xFFEBCD;
const Color Color::Blue					= 0x0000FF;
const Color Color::BlueViolet			= 0x8A2BE2;
const Color Color::Brown				= 0xA52A2A;
const Color Color::BurlyWood			= 0xDEB887;
const Color Color::CadetBlue			= 0x5F9EA0;
const Color Color::Chartreuse			= 0x7FFF00;
const Color Color::Chocolate			= 0xD2691E;
const Color Color::Coral				= 0xFF7F50;
const Color Color::CornflowerBlue		= 0x6495ED;
const Color Color::Cornsilk				= 0xFFF8DC;
const Color Color::Crimson				= 0xDC143C;
const Color Color::Cyan					= 0x00FFFF;
const Color Color::DarkBlue				= 0x00008B;
const Color Color::DarkCyan				= 0x008B8B;
const Color Color::DarkGoldenrod		= 0xB8860B;
const Color Color::DarkGray				= 0xA9A9A9;
const Color Color::DarkGreen			= 0x006400;
const Color Color::DarkKhaki			= 0xBDB76B;
const Color Color::DarkMagenta			= 0x8B008B;
const Color Color::DarkOliveGreen		= 0x556B2F;
const Color Color::DarkOrange			= 0xFF8C00;
const Color Color::DarkOrchid			= 0x9932CC;
const Color Color::DarkRed				= 0x8B0000;
const Color Color::DarkSalmon			= 0xE9967A;
const Color Color::DarkSeaGreen			= 0x8FBC8B;
const Color Color::DarkSlateBlue		= 0x483D8B;
const Color Color::DarkSlateGray		= 0x2F4F4F;
const Color Color::DarkTurquoise		= 0x00CED1;
const Color Color::DarkViolet			= 0x9400D3;
const Color Color::DeepPink				= 0xFF1493;
const Color Color::DeepSkyBlue			= 0x00BFFF;
const Color Color::DimGray				= 0x696969;
const Color Color::DodgerBlue			= 0x1E90FF;
const Color Color::Firebrick			= 0xB22222;
const Color Color::FloralWhite			= 0xFFFAF0;
const Color Color::ForestGreen			= 0x228B22;
const Color Color::Fuchsia				= 0xFF00FF;
const Color Color::Gainsboro			= 0xDCDCDC;
const Color Color::GhostWhite			= 0xF8F8FF;
const Color Color::Gold					= 0xFFD700;
const Color Color::Goldenrod			= 0xDAA520;
const Color Color::Gray					= 0x808080;
const Color Color::Green				= 0x008000;
const Color Color::GreenYellow			= 0xADFF2F;
const Color Color::Honeydew				= 0xF0FFF0;
const Color Color::HotPink				= 0xFF69B4;
const Color Color::IndianRed			= 0xCD5C5C;
const Color Color::Indigo				= 0x4B0082;
const Color Color::Ivory				= 0xFFFFF0;
const Color Color::Khaki				= 0xF0E68C;
const Color Color::Lavender				= 0xE6E6FA;
const Color Color::LavenderBlush		= 0xFFF0F5;
const Color Color::LawnGreen			= 0x7CFC00;
const Color Color::LemonChiffon			= 0xFFFACD;
const Color Color::LightBlue			= 0xADD8E6;
const Color Color::LightCoral			= 0xF08080;
const Color Color::LightCyan			= 0xE0FFFF;
const Color Color::LightGoldenrodYellow = 0xFAFAD2;
const Color Color::LightGray			= 0xD3D3D3;
const Color Color::LightGreen			= 0x90EE90;
const Color Color::LightPink			= 0xFFB6C1;
const Color Color::LightSalmon			= 0xFFA07A;
const Color Color::LightSeaGreen		= 0x20B2AA;
const Color Color::LightSkyBlue			= 0x87CEFA;
const Color Color::LightSlateGray		= 0x778899;
const Color Color::LightSteelBlue		= 0xB0C4DE;
const Color Color::LightYellow			= 0xFFFFE0;
const Color Color::Lime					= 0x00FF00;
const Color Color::LimeGreen			= 0x32CD32;
const Color Color::Linen				= 0xFAF0E6;
const Color Color::Magenta				= 0xFF00FF;
const Color Color::Maroon				= 0x800000;
const Color Color::MediumAquamarine		= 0x66CDAA;
const Color Color::MediumBlue			= 0x0000CD;
const Color Color::MediumOrchid			= 0xBA55D3;
const Color Color::MediumPurple			= 0x9370DB;
const Color Color::MediumSeaGreen		= 0x3CB371;
const Color Color::MediumSlateBlue		= 0x7B68EE;
const Color Color::MediumSpringGreen	= 0x00FA9A;
const Color Color::MediumTurquoise		= 0x48D1CC;
const Color Color::MediumVioletRed		= 0xC71585;
const Color Color::MidnightBlue			= 0x191970;
const Color Color::MintCream			= 0xF5FFFA;
const Color Color::MistyRose			= 0xFFE4E1;
const Color Color::Moccasin				= 0xFFE4B5;
const Color Color::NavajoWhite			= 0xFFDEAD;
const Color Color::Navy					= 0x000080;
const Color Color::OldLace				= 0xFDF5E6;
const Color Color::Olive				= 0x808000;
const Color Color::OliveDrab			= 0x6B8E23;
const Color Color::Orange				= 0xFFA500;
const Color Color::OrangeRed			= 0xFF4500;
const Color Color::Orchid				= 0xDA70D6;
const Color Color::PaleGoldenrod		= 0xEEE8AA;
const Color Color::PaleGreen			= 0x98FB98;
const Color Color::PaleTurquoise		= 0xAFEEEE;
const Color Color::PaleVioletRed		= 0xDB7093;
const Color Color::PapayaWhip			= 0xFFEFD5;
const Color Color::PeachPuff			= 0xFFDAB9;
const Color Color::Peru					= 0xCD853F;
const Color Color::Pink					= 0xFFC0CB;
const Color Color::Plum					= 0xDDA0DD;
const Color Color::PowderBlue			= 0xB0E0E6;
const Color Color::Purple				= 0x800080;
const Color Color::Red					= 0xFF0000;
const Color Color::RosyBrown			= 0xBC8F8F;
const Color Color::RoyalBlue			= 0x4169E1;
const Color Color::SaddleBrown			= 0x8B4513;
const Color Color::Salmon				= 0xFA8072;
const Color Color::SandyBrown			= 0xF4A460;
const Color Color::SeaGreen				= 0x2E8B57;
const Color Color::SeaShell				= 0xFFF5EE;
const Color Color::Sienna				= 0xA0522D;
const Color Color::Silver				= 0xC0C0C0;
const Color Color::SkyBlue				= 0x87CEEB;
const Color Color::SlateBlue			= 0x6A5ACD;
const Color Color::SlateGray			= 0x708090;
const Color Color::Snow					= 0xFFFAFA;
const Color Color::SpringGreen			= 0x00FF7F;
const Color Color::SteelBlue			= 0x4682b4;
const Color Color::Tan					= 0xD2B48C;
const Color Color::Teal					= 0x008080;
const Color Color::Thistle				= 0xD8BFD8;
const Color Color::Tomato				= 0xFF6347;
const Color Color::Turquoise			= 0x40E0D0;
const Color Color::Violet				= 0xEE82EE;
const Color Color::Wheat				= 0xF5DEB3;
const Color Color::White				= 0xFFFFFF;
const Color Color::WhiteSmoke			= 0xF5F5F5;
const Color Color::Yellow				= 0xFFFF00;
const Color Color::YellowGreen			= 0x9ACD32;

std::map<wstring_view, uint32> KeyValueMap
{
	{ L"aliceblue", 0xf0f8ff },
	{ L"antiquewhite", 0xfaebd7 },
	{ L"aqua", 0x00ffff },
	{ L"aquamarine", 0x7fffd4 },
	{ L"azure", 0xf0ffff },
	{ L"beige", 0xf5f5dc },
	{ L"bisque", 0xffe4c4 },
	{ L"black", 0x000000 },
	{ L"blanchedalmond", 0xffebcd },
	{ L"blue", 0x0000ff },
	{ L"blueviolet", 0x8a2be2 },
	{ L"brown", 0xa52a2a },
	{ L"burlywood", 0xdeb887 },
	{ L"cadetblue", 0x5f9ea0 },
	{ L"chartreuse", 0x7fff00 },
	{ L"chocolate", 0xd2691e },
	{ L"coral", 0xff7f50 },
	{ L"cornflowerblue", 0x6495ed },
	{ L"cornsilk", 0xfff8dc },
	{ L"crimson", 0xdc143c },
	{ L"cyan", 0x00ffff },
	{ L"darkblue", 0x00008b },
	{ L"darkcyan", 0x008b8b },
	{ L"darkgoldenrod", 0xb8860b },
	{ L"darkgray", 0xa9a9a9 },
	{ L"darkgreen", 0x006400 },
	{ L"darkkhaki", 0xbdb76b },
	{ L"darkmagenta", 0x8b008b },
	{ L"darkolivegreen", 0x556b2f },
	{ L"darkorange", 0xff8c00 },
	{ L"darkorchid", 0x9932cc },
	{ L"darkred", 0x8b0000 },
	{ L"darksalmon", 0xe9967a },
	{ L"darkseagreen", 0xe9967a },
	{ L"darkslateblue", 0x483d8b },
	{ L"darkslategray", 0x2f4f4f },
	{ L"darkturquoise", 0x00ced1 },
	{ L"darkviolet", 0x9400d3 },
	{ L"deeppink", 0xff1493 },
	{ L"deepskyblue", 0x00bfff },
	{ L"dimgray", 0x696969 },
	{ L"dodgerblue", 0x1e90ff },
	{ L"firebrick", 0xb22222 },
	{ L"floralwhite", 0xfffaf0 },
	{ L"forestgreen", 0x228b22 },
	{ L"fuchsia", 0xff00ff },
	{ L"gainsboro", 0xdcdcdc },
	{ L"ghostwhite", 0xf8f8ff },
	{ L"gold", 0xffd700 },
	{ L"goldenrod", 0xdaa520 },
	{ L"gray", 0x808080 },
	{ L"green", 0x008000 },
	{ L"greenyellow", 0xadff2f },
	{ L"honeydew", 0xf0fff0 },
	{ L"hotpink", 0xff69b4 },
	{ L"indianred", 0xcd5c5c },
	{ L"indigo", 0x4b0082 },
	{ L"ivory", 0xfffff0 },
	{ L"khaki", 0xf0e68c },
	{ L"lavender", 0xe6e6fa },
	{ L"lavenderblush", 0xfff0f5 },
	{ L"lawngreen", 0x7cfc00 },
	{ L"lemonchiffon", 0xfffacd },
	{ L"lightblue", 0xadd8e6 },
	{ L"lightcoral", 0xf08080 },
	{ L"lightcyan", 0xe0ffff },
	{ L"lightgoldenrodyellow", 0xfafad2 },
	{ L"lightgray", 0xd3d3d3 },
	{ L"lightgreen", 0x90ee90 },
	{ L"lightpink", 0xffb6c1 },
	{ L"lightsalmon", 0xffa07a },
	{ L"lightseagreen", 0x20b2aa },
	{ L"lightskyblue", 0x87cefa },
	{ L"lightslategray", 0x778899 },
	{ L"lightsteelblue", 0xb0c4de },
	{ L"lightyellow", 0xffffe0 },
	{ L"lime", 0x00ff00 },
	{ L"limegreen", 0x32cd32 },
	{ L"linen", 0xfaf0e6 },
	{ L"magenta", 0xff00ff },
	{ L"maroon", 0x800000 },
	{ L"mediumaquamarine", 0x66cdaa },
	{ L"mediumblue", 0x0000cd },
	{ L"mediumorchid", 0xba55d3 },
	{ L"mediumpurple", 0x9370db },
	{ L"mediumseagreen", 0x3cb371 },
	{ L"mediumslateblue", 0x7b68ee },
	{ L"mediumspringgreen", 0x00fa9a },
	{ L"mediumturquoise", 0x48d1cc },
	{ L"mediumvioletred", 0xc71585 },
	{ L"midnightblue", 0x191970 },
	{ L"mintcream", 0xf5fffa },
	{ L"mistyrose", 0xffe4e1 },
	{ L"moccasin", 0xffe4b5 },
	{ L"navajowhite", 0xffdead },
	{ L"navy", 0x000080 },
	{ L"oldlace", 0xfdf5e6 },
	{ L"olive", 0x808000 },
	{ L"olivedrab", 0x6b8e23 },
	{ L"orange", 0xffa500 },
	{ L"orangered", 0xff4500 },
	{ L"orchid", 0xda70d6 },
	{ L"palegoldenrod", 0xeee8aa },
	{ L"palegreen", 0x98fb98 },
	{ L"paleturquoise", 0xafeeee },
	{ L"palevioletred", 0xdb7093 },
	{ L"papayawhip", 0xffefd5 },
	{ L"peachpuff", 0xffdab9 },
	{ L"peru", 0xcd853f },
	{ L"pink", 0xffc0cb },
	{ L"plum", 0xdda0dd },
	{ L"powderblue", 0xb0e0e6 },
	{ L"purple", 0x800080 },
	{ L"red", 0xff0000 },
	{ L"rosybrown", 0xbc8f8f },
	{ L"royalblue", 0x4169e1 },
	{ L"saddlebrown", 0x8b4513 },
	{ L"salmon", 0xfa8072 },
	{ L"sandybrown", 0xf4a460 },
	{ L"seagreen", 0x2e8b57 },
	{ L"seashell", 0xfff5ee },
	{ L"sienna", 0xa0522d },
	{ L"silver", 0xc0c0c0 },
	{ L"skyblue", 0x87ceeb },
	{ L"slateblue", 0x6a5acd },
	{ L"slategray", 0x708090 },
	{ L"snow", 0xfffafa },
	{ L"springgreen", 0x00ff7f },
	{ L"steelblue", 0x4682b4 },
	{ L"tan", 0xd2b48c },
	{ L"teal", 0xd2b48c },
	{ L"thistle", 0xd8bfd8 },
	{ L"tomato", 0xff6347 },
	{ L"turquoise", 0x40e0d0 },
	{ L"violet", 0xee82ee },
	{ L"wheat", 0xf5deb3 },
	{ L"white", 0xffffff },
	{ L"whitesmoke", 0xf5f5f5 },
	{ L"yellow", 0xffff00 },
	{ L"yellowgreen", 0x9acd32 }
};

Color::Color() : ValueType()
	, R( 0 ), G( 0 ), B( 0 ), A( 0 )
{

}

Color::Color( const Color& copy ) : Color( copy.R, copy.G, copy.B, copy.A )
{

}

Color::Color( const Color& copy, float alphaOverride ) : Color( copy.R, copy.G, copy.B, alphaOverride )
{

}

Color::Color( float r, float g, float b, float a ) : ValueType()
	, R( r ), G( g ), B( b ), A( a )
{

}

Color::Color( int r, int g, int b, int a ) : ValueType()
{
	IntR_set( r );
	IntG_set( g );
	IntB_set( b );
	IntA_set( a );
}

Color::Color( uint32 rgb, int a ) : Color( ( uint8 )( rgb >> 16 ), ( uint8 )( rgb >> 8 ), ( uint8 )( rgb >> 0 ), a )
{

}

Color::Color( uint32 rgb, float a ) : Color( ( uint8 )( rgb >> 16 ), ( uint8 )( rgb >> 8 ), ( uint8 )( rgb >> 0 ), ( int )( a * 255.0f ) )
{

}

String Color::ToString()
{
	wstringstream wss;
	wss << L"#" << setfill( L'0' ) << right << hex <<
		setw( 2 ) << IntA_get() <<
		setw( 2 ) << IntR_get() <<
		setw( 2 ) << IntG_get() <<
		setw( 2 ) << IntB_get();
	return wss.str().c_str();
}

bool Color::Equals( object right )
{
	if ( Color r; right.Is<Color>( &r ) )
	{
		return Equals( r );
	}
	else
	{
		return false;
	}
}

bool Color::Equals( Color right )
{
	return operator==( right );
}

int Color::CompareTo( Color right )
{
	if ( R == right.R )
		if ( G == right.G )
			if ( B == right.B )
				if ( A == right.A )
					return 0;
				else
					return A < right.A ? -1 : 1;
			else
				return B < right.B ? -1 : 1;
		else
			return G < right.G ? -1 : 1;
	else
		return R < right.R ? -1 : 1;
}

Color Color::AlphaBlend( const Color& right ) const
{
	float invA = 1.0f - right.A;

	Color ret = operator*( invA ) + right * right.A;
	ret.A = right.A;

	return ret;
}

int Color::IntR_get() const
{
	return ( int )( R * 255.0f );
}

void Color::IntR_set( int value )
{
	if ( value <= 0 ) value = 0;
	else if ( value >= 255 ) value = 255;
	R = ( float )value / 255.0f;
}

int Color::IntG_get() const
{
	return ( int )( G * 255.0f );
}

void Color::IntG_set( int value )
{
	if ( value <= 0 ) value = 0;
	else if ( value >= 255 ) value = 255;
	G = ( float )value / 255.0f;
}

int Color::IntB_get() const
{
	return ( int )( B * 255.0f );
}

void Color::IntB_set( int value )
{
	if ( value <= 0 ) value = 0;
	else if ( value >= 255 ) value = 255;
	B = ( float )value / 255.0f;
}

int Color::IntA_get() const
{
	return ( int )( A * 255.0f );
}

void Color::IntA_set( int value )
{
	if ( value <= 0 ) value = 0;
	else if ( value >= 255 ) value = 255;
	A = ( float )value / 255.0f;
}

Color& Color::operator=( const Color& copy )
{
	R = copy.R;
	G = copy.G;
	B = copy.B;
	A = copy.A;
	return *this;
}

Color& Color::operator=( uint32 rgb )
{
	IntR = ( uint8 )( rgb >> 16 );
	IntG = ( uint8 )( rgb >> 8 );
	IntB = ( uint8 )( rgb >> 0 );
	return *this;
}

Color& Color::operator+=( float value )
{
	R += value;
	G += value;
	B += value;
	A += value;
	return *this;
}

Color& Color::operator-=( float value )
{
	R -= value;
	G -= value;
	B -= value;
	A -= value;
	return *this;
}

Color& Color::operator*=( float value )
{
	R *= value;
	G *= value;
	B *= value;
	A *= value;
	return *this;
}

Color& Color::operator/=( float value )
{
	R /= value;
	G /= value;
	B /= value;
	A /= value;
	return *this;
}

Color& Color::operator+=( const Color& value )
{
	R += value.R;
	G += value.G;
	B += value.B;
	A += value.A;
	return *this;
}

Color& Color::operator-=( const Color& value )
{
	R -= value.R;
	G -= value.G;
	B -= value.B;
	A -= value.A;
	return *this;
}

Color& Color::operator*=( const Color& value )
{
	R *= value.R;
	G *= value.G;
	B *= value.B;
	A *= value.A;
	return *this;
}

Color& Color::operator/=( const Color& value )
{
	R /= value.R;
	G /= value.G;
	B /= value.B;
	A /= value.A;
	return *this;
}

Color Color::operator+( float value ) const
{
	return Color( R + value, G + value, B + value, A + value );
}

Color Color::operator-( float value ) const
{
	return Color( R - value, G - value, B - value, A - value );
}

Color Color::operator*( float value ) const
{
	return Color( R * value, G * value, B * value, A * value );
}

Color Color::operator/( float value ) const
{
	return Color( R / value, G / value, B / value, A / value );
}

Color Color::operator+( const Color& copy ) const
{
	return Color( R + copy.R, G + copy.G, B + copy.B, A + copy.A );
}

Color Color::operator-( const Color& copy ) const
{
	return Color( R - copy.R, G - copy.G, B - copy.B, A - copy.A );
}

Color Color::operator*( const Color& copy ) const
{
	return Color( R * copy.R, G * copy.G, B * copy.B, A * copy.A );
}

Color Color::operator/( const Color& copy ) const
{
	return Color( R / copy.R, G / copy.G, B / copy.B, A / copy.A );
}

bool Color::operator==( const Color& right ) const
{
	return R == right.R && G == right.G && B == right.B && A == right.A;
}

bool Color::operator!=( const Color& right ) const
{
	return R != right.R || G != right.G || B != right.B || A != right.A;
}

bool Color::operator< ( const Color& right ) const
{
	if ( R < right.R ) return true;
	else if ( R > right.R ) return false;
	else if ( G < right.G ) return true;
	else if ( G > right.G ) return false;
	else if ( B < right.B ) return true;
	else if ( B > right.B ) return false;
	else if ( A < right.A ) return true;
	else return false;
}

Color Color::TryParse( String argbCode )
{
	Color c;

	if ( argbCode.Chars[0] == L'#' )
	{
		if ( argbCode.Length >= 9 )  // 8磊府 内靛
		{
			c.IntR = stoi( wstring( argbCode.Chars + 3, 2 ), nullptr, 16 );
			c.IntG = stoi( wstring( argbCode.Chars + 5, 2 ), nullptr, 16 );
			c.IntB = stoi( wstring( argbCode.Chars + 7, 2 ), nullptr, 16 );
			c.IntA = stoi( wstring( argbCode.Chars + 1, 2 ), nullptr, 16 );
		}
		else if ( argbCode.Length >= 7 )  // 6磊府 内靛
		{
			c.IntR = stoi( wstring( argbCode.Chars + 1, 2 ), nullptr, 16 );
			c.IntG = stoi( wstring( argbCode.Chars + 3, 2 ), nullptr, 16 );
			c.IntB = stoi( wstring( argbCode.Chars + 5, 2 ), nullptr, 16 );
			c.A = 1.0f;
		}
	}
	else
	{
		return KeyValueMap[argbCode.Lowercase.Chars];
	}

	return c;
}