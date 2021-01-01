// Copyright 2020 Aumoa.lib. All right reserved.

#include "Numerics/Color.h"

#include "CoreString.h"
#include "HashHelper.h"
#include "IndexOutOfRangeException.h"

using namespace std;

map<size_t, Color> Color::convertTable =
{
	{ String("AliceBlue").GetHashCode(), Color::FromHtml("#F0F8FF") },
	{ String("AntiqueWhite").GetHashCode(), Color::FromHtml("#FAEBD7") },
	{ String("Aqua").GetHashCode(), Color::FromHtml("#00FFFF") },
	{ String("Aquamarine").GetHashCode(), Color::FromHtml("#7FFFD4") },
	{ String("Azure").GetHashCode(), Color::FromHtml("#F0FFFF") },
	{ String("Beige").GetHashCode(), Color::FromHtml("#F5F5DC") },
	{ String("Bisque").GetHashCode(), Color::FromHtml("#FFE4C4") },
	{ String("Black").GetHashCode(), Color::FromHtml("#000000") },
	{ String("BlanchedAlmond").GetHashCode(), Color::FromHtml("#FFEBCD") },
	{ String("Blue").GetHashCode(), Color::FromHtml("#0000FF") },
	{ String("BlueViolet").GetHashCode(), Color::FromHtml("#8A2BE2") },
	{ String("Brown").GetHashCode(), Color::FromHtml("#A52A2A") },
	{ String("BurlyWood").GetHashCode(), Color::FromHtml("#DEB887") },
	{ String("CadetBlue").GetHashCode(), Color::FromHtml("#5F9EA0") },
	{ String("Chartreuse").GetHashCode(), Color::FromHtml("#7FFF00") },
	{ String("Chocolate").GetHashCode(), Color::FromHtml("#D2691E") },
	{ String("Coral").GetHashCode(), Color::FromHtml("#FF7F50") },
	{ String("CornflowerBlue").GetHashCode(), Color::FromHtml("#6495ED") },
	{ String("Cornsilk").GetHashCode(), Color::FromHtml("#FFF8DC") },
	{ String("Crimson").GetHashCode(), Color::FromHtml("#DC143C") },
	{ String("Cyan").GetHashCode(), Color::FromHtml("#00FFFF") },
	{ String("DarkBlue").GetHashCode(), Color::FromHtml("#00008B") },
	{ String("DarkCyan").GetHashCode(), Color::FromHtml("#008B8B") },
	{ String("DarkGoldenrod").GetHashCode(), Color::FromHtml("#B8860B") },
	{ String("DarkGray").GetHashCode(), Color::FromHtml("#A9A9A9") },
	{ String("DarkGreen").GetHashCode(), Color::FromHtml("#006400") },
	{ String("DarkKhaki").GetHashCode(), Color::FromHtml("#BDB76B") },
	{ String("DarkMagenta").GetHashCode(), Color::FromHtml("#8B008B") },
	{ String("DarkOliveGreen").GetHashCode(), Color::FromHtml("#556B2F") },
	{ String("DarkOrange").GetHashCode(), Color::FromHtml("#FF8C00") },
	{ String("DarkOrchid").GetHashCode(), Color::FromHtml("#9932CC") },
	{ String("DarkRed").GetHashCode(), Color::FromHtml("#8B0000") },
	{ String("DarkSalmon").GetHashCode(), Color::FromHtml("#E9967A") },
	{ String("DarkSeaGreen").GetHashCode(), Color::FromHtml("#8FBC8B") },
	{ String("DarkSlateBlue").GetHashCode(), Color::FromHtml("#483D8B") },
	{ String("DarkSlateGray").GetHashCode(), Color::FromHtml("#2F4F4F") },
	{ String("DarkTurquoise").GetHashCode(), Color::FromHtml("#00CED1") },
	{ String("DarkViolet").GetHashCode(), Color::FromHtml("#9400D3") },
	{ String("DeepPink").GetHashCode(), Color::FromHtml("#FF1493") },
	{ String("DeepSkyBlue").GetHashCode(), Color::FromHtml("#00BFFF") },
	{ String("DimGray").GetHashCode(), Color::FromHtml("#696969") },
	{ String("DodgerBlue").GetHashCode(), Color::FromHtml("#1E90FF") },
	{ String("Firebrick").GetHashCode(), Color::FromHtml("#B22222") },
	{ String("FloralWhite").GetHashCode(), Color::FromHtml("#FFFAF0") },
	{ String("ForestGreen").GetHashCode(), Color::FromHtml("#228B22") },
	{ String("Fuchsia").GetHashCode(), Color::FromHtml("#FF00FF") },
	{ String("Gainsboro").GetHashCode(), Color::FromHtml("#DCDCDC") },
	{ String("GhostWhite").GetHashCode(), Color::FromHtml("#F8F8FF") },
	{ String("Gold").GetHashCode(), Color::FromHtml("#FFD700") },
	{ String("Goldenrod").GetHashCode(), Color::FromHtml("#DAA520") },
	{ String("Gray").GetHashCode(), Color::FromHtml("#808080") },
	{ String("Green").GetHashCode(), Color::FromHtml("#008000") },
	{ String("GreenYellow").GetHashCode(), Color::FromHtml("#ADFF2F") },
	{ String("Honeydew").GetHashCode(), Color::FromHtml("#F0FFF0") },
	{ String("HotPink").GetHashCode(), Color::FromHtml("#FF69B4") },
	{ String("IndianRed").GetHashCode(), Color::FromHtml("#CD5C5C") },
	{ String("Indigo").GetHashCode(), Color::FromHtml("#4B0082") },
	{ String("Ivory").GetHashCode(), Color::FromHtml("#FFFFF0") },
	{ String("Khaki").GetHashCode(), Color::FromHtml("#F0E68C") },
	{ String("Lavender").GetHashCode(), Color::FromHtml("#E6E6FA") },
	{ String("LavenderBlush").GetHashCode(), Color::FromHtml("#FFF0F5") },
	{ String("LawnGreen").GetHashCode(), Color::FromHtml("#7CFC00") },
	{ String("LemonChiffon").GetHashCode(), Color::FromHtml("#FFFACD") },
	{ String("LightBlue").GetHashCode(), Color::FromHtml("#ADD8E6") },
	{ String("LightCoral").GetHashCode(), Color::FromHtml("#F08080") },
	{ String("LightCyan").GetHashCode(), Color::FromHtml("#E0FFFF") },
	{ String("LightGoldenrodYellow").GetHashCode(), Color::FromHtml("#FAFAD2") },
	{ String("LightGray").GetHashCode(), Color::FromHtml("#D3D3D3") },
	{ String("LightGreen").GetHashCode(), Color::FromHtml("#90EE90") },
	{ String("LightPink").GetHashCode(), Color::FromHtml("#FFB6C1") },
	{ String("LightSalmon").GetHashCode(), Color::FromHtml("#FFA07A") },
	{ String("LightSeaGreen").GetHashCode(), Color::FromHtml("#20B2AA") },
	{ String("LightSkyBlue").GetHashCode(), Color::FromHtml("#87CEFA") },
	{ String("LightSlateGray").GetHashCode(), Color::FromHtml("#778899") },
	{ String("LightSteelBlue").GetHashCode(), Color::FromHtml("#B0C4DE") },
	{ String("LightYellow").GetHashCode(), Color::FromHtml("#FFFFE0") },
	{ String("Lime").GetHashCode(), Color::FromHtml("#00FF00") },
	{ String("LimeGreen").GetHashCode(), Color::FromHtml("#32CD32") },
	{ String("Linen").GetHashCode(), Color::FromHtml("#FAF0E6") },
	{ String("Magenta").GetHashCode(), Color::FromHtml("#FF00FF") },
	{ String("Maroon").GetHashCode(), Color::FromHtml("#800000") },
	{ String("MediumAquamarine").GetHashCode(), Color::FromHtml("#66CDAA") },
	{ String("MediumBlue").GetHashCode(), Color::FromHtml("#0000CD") },
	{ String("MediumOrchid").GetHashCode(), Color::FromHtml("#BA55D3") },
	{ String("MediumPurple").GetHashCode(), Color::FromHtml("#9370DB") },
	{ String("MediumSeaGreen").GetHashCode(), Color::FromHtml("#3CB371") },
	{ String("MediumSlateBlue").GetHashCode(), Color::FromHtml("#7B68EE") },
	{ String("MediumSpringGreen").GetHashCode(), Color::FromHtml("#00FA9A") },
	{ String("MediumTurquoise").GetHashCode(), Color::FromHtml("#48D1CC") },
	{ String("MediumVioletRed").GetHashCode(), Color::FromHtml("#C71585") },
	{ String("MidnightBlue").GetHashCode(), Color::FromHtml("#191970") },
	{ String("MintCream").GetHashCode(), Color::FromHtml("#F5FFFA") },
	{ String("MistyRose").GetHashCode(), Color::FromHtml("#FFE4E1") },
	{ String("Moccasin").GetHashCode(), Color::FromHtml("#FFE4B5") },
	{ String("NavajoWhite").GetHashCode(), Color::FromHtml("#FFDEAD") },
	{ String("Navy").GetHashCode(), Color::FromHtml("#000080") },
	{ String("OldLace").GetHashCode(), Color::FromHtml("#FDF5E6") },
	{ String("Olive").GetHashCode(), Color::FromHtml("#808000") },
	{ String("OliveDrab").GetHashCode(), Color::FromHtml("#6B8E23") },
	{ String("Orange").GetHashCode(), Color::FromHtml("#FFA500") },
	{ String("OrangeRed").GetHashCode(), Color::FromHtml("#FF4500") },
	{ String("Orchid").GetHashCode(), Color::FromHtml("#DA70D6") },
	{ String("PaleGoldenrod").GetHashCode(), Color::FromHtml("#EEE8AA") },
	{ String("PaleGreen").GetHashCode(), Color::FromHtml("#98FB98") },
	{ String("PaleTurquoise").GetHashCode(), Color::FromHtml("#AFEEEE") },
	{ String("PaleVioletRed").GetHashCode(), Color::FromHtml("#DB7093") },
	{ String("PapayaWhip").GetHashCode(), Color::FromHtml("#FFEFD5") },
	{ String("PeachPuff").GetHashCode(), Color::FromHtml("#FFDAB9") },
	{ String("Peru").GetHashCode(), Color::FromHtml("#CD853F") },
	{ String("Pink").GetHashCode(), Color::FromHtml("#FFC0CB") },
	{ String("Plum").GetHashCode(), Color::FromHtml("#DDA0DD") },
	{ String("PowderBlue").GetHashCode(), Color::FromHtml("#B0E0E6") },
	{ String("Purple").GetHashCode(), Color::FromHtml("#800080") },
	{ String("Red").GetHashCode(), Color::FromHtml("#FF0000") },
	{ String("RosyBrown").GetHashCode(), Color::FromHtml("#BC8F8F") },
	{ String("RoyalBlue").GetHashCode(), Color::FromHtml("#4169E1") },
	{ String("SaddleBrown").GetHashCode(), Color::FromHtml("#8B4513") },
	{ String("Salmon").GetHashCode(), Color::FromHtml("#FA8072") },
	{ String("SandyBrown").GetHashCode(), Color::FromHtml("#F4A460") },
	{ String("SeaGreen").GetHashCode(), Color::FromHtml("#2E8B57") },
	{ String("SeaShell").GetHashCode(), Color::FromHtml("#FFF5EE") },
	{ String("Sienna").GetHashCode(), Color::FromHtml("#A0522D") },
	{ String("Silver").GetHashCode(), Color::FromHtml("#C0C0C0") },
	{ String("SkyBlue").GetHashCode(), Color::FromHtml("#87CEEB") },
	{ String("SlateBlue").GetHashCode(), Color::FromHtml("#6A5ACD") },
	{ String("SlateGray").GetHashCode(), Color::FromHtml("#708090") },
	{ String("Snow").GetHashCode(), Color::FromHtml("#FFFAFA") },
	{ String("SpringGreen").GetHashCode(), Color::FromHtml("#00FF7F") },
	{ String("SteelBlue").GetHashCode(), Color::FromHtml("#4682B4") },
	{ String("Tan").GetHashCode(), Color::FromHtml("#D2B48C") },
	{ String("Teal").GetHashCode(), Color::FromHtml("#008080") },
	{ String("Thistle").GetHashCode(), Color::FromHtml("#D8BFD8") },
	{ String("Tomato").GetHashCode(), Color::FromHtml("#FF6347") },
	{ String("Transparent").GetHashCode(), Color::FromHtml("#00000000") },
	{ String("Turquoise").GetHashCode(), Color::FromHtml("#40E0D0") },
	{ String("Violet").GetHashCode(), Color::FromHtml("#EE82EE") },
	{ String("Wheat").GetHashCode(), Color::FromHtml("#F5DEB3") },
	{ String("White").GetHashCode(), Color::FromHtml("#FFFFFF") },
	{ String("WhiteSmoke").GetHashCode(), Color::FromHtml("#F5F5F5") },
	{ String("Yellow").GetHashCode(), Color::FromHtml("#FFFF00") },
	{ String("YellowGreen").GetHashCode(), Color::FromHtml("#9ACD32") },
};

Color::Color(float splat) : This(splat, splat, splat, splat)
{

}

Color::Color()
{

}

Color::Color(float r, float g, float b, float a)
{
	R = r;
	G = g;
	B = b;
	A = a;
}

Color::Color(const Color& copy)
{
	R = copy.R;
	G = copy.G;
	B = copy.B;
	A = copy.A;
}

bool Color::Equals(const Color& rh) const
{
	return (*this) == rh;
}

bool Color::NearlyEquals(const Color& rh, float epsilon) const
{
	return abs(R - rh.R) <= epsilon
		&& abs(G - rh.G) <= epsilon
		&& abs(B - rh.B) <= epsilon
		&& abs(A - rh.A) <= epsilon;
}

size_t Color::GetHashCode() const
{
	return HashHelper::GetHashCode(R)
		 ^ HashHelper::GetHashCode(G)
		 ^ HashHelper::GetHashCode(B)
		 ^ HashHelper::GetHashCode(A);
}

TRefPtr<String> Color::ToString() const
{
	return String::Format("{{R: {0}, G: {1}, B: {2}, A: {3}}}", R, G, B, A);
}

float Color::GetComponentOrDefault(size_t index) const
{
	if (!Contains(index))
	{
		return 0;
	}

	const float* ptr = &R;
	return ptr[index];
}

bool Color::Contains(size_t index) const
{
	return index < Count;
}

size_t Color::Count_get() const
{
	return 3;
}

const float& Color::operator [](size_t index) const
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	const float* ptr = &R;
	return ptr[index];
}

float& Color::operator [](size_t index)
{
	if (!Contains(index))
	{
		throw IndexOutOfRangeException();
	}

	float* ptr = &R;
	return ptr[index];
}

Color Color::operator -() const
{
	return Color(-R, -G, -B, -A);
}

Color Color::operator +(const Color& value) const
{
	return Color(R + value.R, G + value.G, B + value.B, A + value.A);
}

Color Color::operator -(const Color& value) const
{
	return Color(R - value.R, G - value.G, B - value.B, A + value.A);
}

Color Color::operator *(const Color& value) const
{
	return Color(R * value.R, G * value.G, B * value.B, A + value.A);
}

Color Color::operator /(const Color& value) const
{
	return Color(R / value.R, G / value.G, B / value.B, A + value.A);
}

Color Color::operator ^(const Color& value) const
{
	return AlphaBlend(*this, value);
}

bool Color::operator ==(const Color& value) const
{
	return R == value.R && G == value.G && B == value.B && A == value.A;
}

bool Color::operator !=(const Color& value) const
{
	return R != value.R || G != value.G || B != value.B || A != value.A;
}

bool Color::operator < (const Color& right) const
{
	if (R < right.R)
	{
		return true;
	}
	else if (R > right.R)
	{
		return false;
	}
	else if (G < right.G)
	{
		return true;
	}
	else if (G > right.G)
	{
		return false;
	}
	else if (B < right.B)
	{
		return true;
	}
	else if (B > right.B)
	{
		return false;
	}
	else if (A < right.A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Color::operator <=(const Color& right) const
{
	if (R < right.R)
	{
		return true;
	}
	else if (R > right.R)
	{
		return false;
	}
	else if (G < right.G)
	{
		return true;
	}
	else if (G > right.G)
	{
		return false;
	}
	else if (B < right.B)
	{
		return true;
	}
	else if (B > right.B)
	{
		return false;
	}
	else if (A <= right.A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Color::operator > (const Color& right) const
{
	if (R > right.R)
	{
		return true;
	}
	else if (R < right.R)
	{
		return false;
	}
	else if (G > right.G)
	{
		return true;
	}
	else if (G < right.G)
	{
		return false;
	}
	else if (B > right.B)
	{
		return true;
	}
	else if (B < right.B)
	{
		return false;
	}
	else if (A > right.A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Color::operator >=(const Color& right) const
{
	if (R > right.R)
	{
		return true;
	}
	else if (R < right.R)
	{
		return false;
	}
	else if (G > right.G)
	{
		return true;
	}
	else if (G < right.G)
	{
		return false;
	}
	else if (B > right.B)
	{
		return true;
	}
	else if (B < right.B)
	{
		return false;
	}
	else if (A >= right.A)
	{
		return true;
	}
	else
	{
		return false;
	}
}

weak_ordering Color::operator <=>(const Color& right) const
{
	if (R < right.R)
	{
		return weak_ordering::less;
	}
	else if (R > right.R)
	{
		return weak_ordering::greater;
	}
	else if (G < right.G)
	{
		return weak_ordering::less;
	}
	else if (G > right.G)
	{
		return weak_ordering::greater;
	}
	else if (B < right.B)
	{
		return weak_ordering::less;
	}
	else if (B > right.B)
	{
		return weak_ordering::greater;
	}
	else if (A < right.A)
	{
		return weak_ordering::less;
	}
	else if (A > right.A)
	{
		return weak_ordering::greater;
	}
	else
	{
		return weak_ordering::equivalent;
	}
}

Color& Color::operator +=(const Color& right)
{
	R += right.R;
	G += right.G;
	B += right.B;
	A += right.A;
	return *this;
}

Color& Color::operator -=(const Color& right)
{
	R -= right.R;
	G -= right.G;
	B -= right.B;
	A -= right.A;
	return *this;
}

Color& Color::operator *=(const Color& right)
{
	R *= right.R;
	G *= right.G;
	B *= right.B;
	A *= right.A;
	return *this;
}

Color& Color::operator /=(const Color& right)
{
	R /= right.R;
	G /= right.G;
	B /= right.B;
	A /= right.A;
	return *this;
}

Color& Color::operator ^=(const Color& right)
{
	return (*this) = (*this) ^ right;
}

Color Color::AlphaBlend(const Color& left, const Color& right)
{
	return AlphaBlend(left, right, right.A);
}

Color Color::AlphaBlend(const Color& left, const Color& right, float alpha)
{
	Color r = left * (1.0f - alpha) + right * alpha;
	r.A = 1.0f;
	return r;
}

Color Color::FromUInt(uint32 argb)
{
	uint8 b = (uint8)argb;
	uint8 g = (uint8)(argb >> 8);
	uint8 r = (uint8)(argb >> 16);
	uint8 a = (uint8)(argb >> 24);
	return FromBytes(r, g, b, a);
}

Color Color::FromHtml(TRefPtr<String> html)
{
	if ((html.IsValid) || (html->Length == 0))
	{
		return Color::Transparent;
	}

	// #AARRGGBB
	if (html->Length == 7)
	{
		wistringstream wiss;
		wiss.str(html->Substring(1, 6)->C_Str);
		uint32 hexCode;
		wiss >> hex >> hexCode;
		return Color::FromUInt(hexCode);
	}

	// #RRGGBB
	if (html->Length == 5)
	{
		wistringstream wiss;
		wiss.str(html->Substring(1, 4)->C_Str);
		uint32 hexCode;
		wiss >> hex >> hexCode;
		hexCode |= 0xFF000000;
		return Color::FromUInt(hexCode);
	}

	// In table color.
	auto it = convertTable.find(html->GetHashCode());
	if (it != convertTable.end())
	{
		return it->second;
	}

	return Color::Transparent;
}

Color Color::FromBytes(uint8 r, uint8 g, uint8 b, uint8 a)
{
	constexpr float Inv255 = 1.0f / 255.0f;
	return Color((float)r * Inv255, (float)g * Inv255, (float)b * Inv255, (float)a * Inv255);
}

Color operator +(float left, const Color& right)
{
	return Color(left, left, left, left) + right;
}

Color operator -(float left, const Color& right)
{
	return Color(left, left, left, left) - right;
}

Color operator *(float left, const Color& right)
{
	return Color(left, left, left, left) * right;
}

Color operator /(float left, const Color& right)
{
	return Color(left, left, left, left) / right;
}

Color Color::AliceBlue = FromHtml("#F0F8FF");
Color Color::AntiqueWhite = FromHtml("#FAEBD7");
Color Color::Aqua = FromHtml("#00FFFF");
Color Color::Aquamarine = FromHtml("#7FFFD4");
Color Color::Azure = FromHtml("#F0FFFF");
Color Color::Beige = FromHtml("#F5F5DC");
Color Color::Bisque = FromHtml("#FFE4C4");
Color Color::Black = FromHtml("#000000");
Color Color::BlanchedAlmond = FromHtml("#FFEBCD");
Color Color::Blue = FromHtml("#0000FF");
Color Color::BlueViolet = FromHtml("#8A2BE2");
Color Color::Brown = FromHtml("#A52A2A");
Color Color::BurlyWood = FromHtml("#DEB887");
Color Color::CadetBlue = FromHtml("#5F9EA0");
Color Color::Chartreuse = FromHtml("#7FFF00");
Color Color::Chocolate = FromHtml("#D2691E");
Color Color::Coral = FromHtml("#FF7F50");
Color Color::CornflowerBlue = FromHtml("#6495ED");
Color Color::Cornsilk = FromHtml("#FFF8DC");
Color Color::Crimson = FromHtml("#DC143C");
Color Color::Cyan = FromHtml("#00FFFF");
Color Color::DarkBlue = FromHtml("#00008B");
Color Color::DarkCyan = FromHtml("#008B8B");
Color Color::DarkGoldenrod = FromHtml("#B8860B");
Color Color::DarkGray = FromHtml("#A9A9A9");
Color Color::DarkGreen = FromHtml("#006400");
Color Color::DarkKhaki = FromHtml("#BDB76B");
Color Color::DarkMagenta = FromHtml("#8B008B");
Color Color::DarkOliveGreen = FromHtml("#556B2F");
Color Color::DarkOrange = FromHtml("#FF8C00");
Color Color::DarkOrchid = FromHtml("#9932CC");
Color Color::DarkRed = FromHtml("#8B0000");
Color Color::DarkSalmon = FromHtml("#E9967A");
Color Color::DarkSeaGreen = FromHtml("#8FBC8B");
Color Color::DarkSlateBlue = FromHtml("#483D8B");
Color Color::DarkSlateGray = FromHtml("#2F4F4F");
Color Color::DarkTurquoise = FromHtml("#00CED1");
Color Color::DarkViolet = FromHtml("#9400D3");
Color Color::DeepPink = FromHtml("#FF1493");
Color Color::DeepSkyBlue = FromHtml("#00BFFF");
Color Color::DimGray = FromHtml("#696969");
Color Color::DodgerBlue = FromHtml("#1E90FF");
Color Color::Firebrick = FromHtml("#B22222");
Color Color::FloralWhite = FromHtml("#FFFAF0");
Color Color::ForestGreen = FromHtml("#228B22");
Color Color::Fuchsia = FromHtml("#FF00FF");
Color Color::Gainsboro = FromHtml("#DCDCDC");
Color Color::GhostWhite = FromHtml("#F8F8FF");
Color Color::Gold = FromHtml("#FFD700");
Color Color::Goldenrod = FromHtml("#DAA520");
Color Color::Gray = FromHtml("#808080");
Color Color::Green = FromHtml("#008000");
Color Color::GreenYellow = FromHtml("#ADFF2F");
Color Color::Honeydew = FromHtml("#F0FFF0");
Color Color::HotPink = FromHtml("#FF69B4");
Color Color::IndianRed = FromHtml("#CD5C5C");
Color Color::Indigo = FromHtml("#4B0082");
Color Color::Ivory = FromHtml("#FFFFF0");
Color Color::Khaki = FromHtml("#F0E68C");
Color Color::Lavender = FromHtml("#E6E6FA");
Color Color::LavenderBlush = FromHtml("#FFF0F5");
Color Color::LawnGreen = FromHtml("#7CFC00");
Color Color::LemonChiffon = FromHtml("#FFFACD");
Color Color::LightBlue = FromHtml("#ADD8E6");
Color Color::LightCoral = FromHtml("#F08080");
Color Color::LightCyan = FromHtml("#E0FFFF");
Color Color::LightGoldenrodYellow = FromHtml("#FAFAD2");
Color Color::LightGray = FromHtml("#D3D3D3");
Color Color::LightGreen = FromHtml("#90EE90");
Color Color::LightPink = FromHtml("#FFB6C1");
Color Color::LightSalmon = FromHtml("#FFA07A");
Color Color::LightSeaGreen = FromHtml("#20B2AA");
Color Color::LightSkyBlue = FromHtml("#87CEFA");
Color Color::LightSlateGray = FromHtml("#778899");
Color Color::LightSteelBlue = FromHtml("#B0C4DE");
Color Color::LightYellow = FromHtml("#FFFFE0");
Color Color::Lime = FromHtml("#00FF00");
Color Color::LimeGreen = FromHtml("#32CD32");
Color Color::Linen = FromHtml("#FAF0E6");
Color Color::Magenta = FromHtml("#FF00FF");
Color Color::Maroon = FromHtml("#800000");
Color Color::MediumAquamarine = FromHtml("#66CDAA");
Color Color::MediumBlue = FromHtml("#0000CD");
Color Color::MediumOrchid = FromHtml("#BA55D3");
Color Color::MediumPurple = FromHtml("#9370DB");
Color Color::MediumSeaGreen = FromHtml("#3CB371");
Color Color::MediumSlateBlue = FromHtml("#7B68EE");
Color Color::MediumSpringGreen = FromHtml("#00FA9A");
Color Color::MediumTurquoise = FromHtml("#48D1CC");
Color Color::MediumVioletRed = FromHtml("#C71585");
Color Color::MidnightBlue = FromHtml("#191970");
Color Color::MintCream = FromHtml("#F5FFFA");
Color Color::MistyRose = FromHtml("#FFE4E1");
Color Color::Moccasin = FromHtml("#FFE4B5");
Color Color::NavajoWhite = FromHtml("#FFDEAD");
Color Color::Navy = FromHtml("#000080");
Color Color::OldLace = FromHtml("#FDF5E6");
Color Color::Olive = FromHtml("#808000");
Color Color::OliveDrab = FromHtml("#6B8E23");
Color Color::Orange = FromHtml("#FFA500");
Color Color::OrangeRed = FromHtml("#FF4500");
Color Color::Orchid = FromHtml("#DA70D6");
Color Color::PaleGoldenrod = FromHtml("#EEE8AA");
Color Color::PaleGreen = FromHtml("#98FB98");
Color Color::PaleTurquoise = FromHtml("#AFEEEE");
Color Color::PaleVioletRed = FromHtml("#DB7093");
Color Color::PapayaWhip = FromHtml("#FFEFD5");
Color Color::PeachPuff = FromHtml("#FFDAB9");
Color Color::Peru = FromHtml("#CD853F");
Color Color::Pink = FromHtml("#FFC0CB");
Color Color::Plum = FromHtml("#DDA0DD");
Color Color::PowderBlue = FromHtml("#B0E0E6");
Color Color::Purple = FromHtml("#800080");
Color Color::Red = FromHtml("#FF0000");
Color Color::RosyBrown = FromHtml("#BC8F8F");
Color Color::RoyalBlue = FromHtml("#4169E1");
Color Color::SaddleBrown = FromHtml("#8B4513");
Color Color::Salmon = FromHtml("#FA8072");
Color Color::SandyBrown = FromHtml("#F4A460");
Color Color::SeaGreen = FromHtml("#2E8B57");
Color Color::SeaShell = FromHtml("#FFF5EE");
Color Color::Sienna = FromHtml("#A0522D");
Color Color::Silver = FromHtml("#C0C0C0");
Color Color::SkyBlue = FromHtml("#87CEEB");
Color Color::SlateBlue = FromHtml("#6A5ACD");
Color Color::SlateGray = FromHtml("#708090");
Color Color::Snow = FromHtml("#FFFAFA");
Color Color::SpringGreen = FromHtml("#00FF7F");
Color Color::SteelBlue = FromHtml("#4682B4");
Color Color::Tan = FromHtml("#D2B48C");
Color Color::Teal = FromHtml("#008080");
Color Color::Thistle = FromHtml("#D8BFD8");
Color Color::Tomato = FromHtml("#FF6347");
Color Color::Transparent = FromHtml("#00000000");
Color Color::Turquoise = FromHtml("#40E0D0");
Color Color::Violet = FromHtml("#EE82EE");
Color Color::Wheat = FromHtml("#F5DEB3");
Color Color::White = FromHtml("#FFFFFF");
Color Color::WhiteSmoke = FromHtml("#F5F5F5");
Color Color::Yellow = FromHtml("#FFFF00");
Color Color::YellowGreen = FromHtml("#9ACD32");