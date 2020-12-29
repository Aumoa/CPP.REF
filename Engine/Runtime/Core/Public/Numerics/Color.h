// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include <compare>
#include <map>
#include "TRefPtr.h"

#pragma pack(push, 8)
struct CORE_API Color
{
	using This = Color;

private:
	Color(double splat);

	static std::map<size_t, Color> convertTable;

public:
	double R;
	double G;
	double B;
	double A;

	Color();
	Color(double r, double g, double b, double a = 1);
	Color(const Color& copy);

	bool Equals(const Color& rh) const;
	bool NearlyEquals(const Color& rh, double epsilon) const;
	size_t GetHashCode() const;
	TRefPtr<String> ToString() const;

	double GetComponentOrDefault(size_t index) const;
	template<TIsVectorType T>
	inline void Construct(const T& vector);
	template<TIsVectorType T>
	inline T Cast() const;
	bool Contains(size_t index) const;

	vs_property_get(size_t, Count);
	size_t Count_get() const;

	const double& operator [](size_t index) const;
	double& operator [](size_t index);

	Color operator -() const;
	Color operator +(const Color& right) const;
	Color operator -(const Color& right) const;
	Color operator *(const Color& right) const;
	Color operator /(const Color& right) const;
	Color operator ^(const Color& right) const;

	bool operator ==(const Color& right) const;
	bool operator !=(const Color& right) const;
	bool operator < (const Color& right) const;
	bool operator <=(const Color& right) const;
	bool operator > (const Color& right) const;
	bool operator >=(const Color& right) const;
	std::weak_ordering operator <=>(const Color& right) const;

	Color& operator +=(const Color& right);
	Color& operator -=(const Color& right);
	Color& operator *=(const Color& right);
	Color& operator /=(const Color& right);
	Color& operator ^=(const Color& right);

	static Color AlphaBlend(const Color& left, const Color& right);
	static Color AlphaBlend(const Color& left, const Color& right, double alpha);
	static Color FromUInt(uint32 argb);
	static Color FromHtml(TRefPtr<String> html);
	static Color FromBytes(uint8 r, uint8 g, uint8 b, uint8 a);
		
	static Color AliceBlue;
	static Color AntiqueWhite;
	static Color Aqua;
	static Color Aquamarine;
	static Color Azure;
	static Color Beige;
	static Color Bisque;
	static Color Black;
	static Color BlanchedAlmond;
	static Color Blue;
	static Color BlueViolet;
	static Color Brown;
	static Color BurlyWood;
	static Color CadetBlue;
	static Color Chartreuse;
	static Color Chocolate;
	static Color Coral;
	static Color CornflowerBlue;
	static Color Cornsilk;
	static Color Crimson;
	static Color Cyan;
	static Color DarkBlue;
	static Color DarkCyan;
	static Color DarkGoldenrod;
	static Color DarkGray;
	static Color DarkGreen;
	static Color DarkKhaki;
	static Color DarkMagenta;
	static Color DarkOliveGreen;
	static Color DarkOrange;
	static Color DarkOrchid;
	static Color DarkRed;
	static Color DarkSalmon;
	static Color DarkSeaGreen;
	static Color DarkSlateBlue;
	static Color DarkSlateGray;
	static Color DarkTurquoise;
	static Color DarkViolet;
	static Color DeepPink;
	static Color DeepSkyBlue;
	static Color DimGray;
	static Color DodgerBlue;
	static Color Firebrick;
	static Color FloralWhite;
	static Color ForestGreen;
	static Color Fuchsia;
	static Color Gainsboro;
	static Color GhostWhite;
	static Color Gold;
	static Color Goldenrod;
	static Color Gray;
	static Color Green;
	static Color GreenYellow;
	static Color Honeydew;
	static Color HotPink;
	static Color IndianRed;
	static Color Indigo;
	static Color Ivory;
	static Color Khaki;
	static Color Lavender;
	static Color LavenderBlush;
	static Color LawnGreen;
	static Color LemonChiffon;
	static Color LightBlue;
	static Color LightCoral;
	static Color LightCyan;
	static Color LightGoldenrodYellow;
	static Color LightGray;
	static Color LightGreen;
	static Color LightPink;
	static Color LightSalmon;
	static Color LightSeaGreen;
	static Color LightSkyBlue;
	static Color LightSlateGray;
	static Color LightSteelBlue;
	static Color LightYellow;
	static Color Lime;
	static Color LimeGreen;
	static Color Linen;
	static Color Magenta;
	static Color Maroon;
	static Color MediumAquamarine;
	static Color MediumBlue;
	static Color MediumOrchid;
	static Color MediumPurple;
	static Color MediumSeaGreen;
	static Color MediumSlateBlue;
	static Color MediumSpringGreen;
	static Color MediumTurquoise;
	static Color MediumVioletRed;
	static Color MidnightBlue;
	static Color MintCream;
	static Color MistyRose;
	static Color Moccasin;
	static Color NavajoWhite;
	static Color Navy;
	static Color OldLace;
	static Color Olive;
	static Color OliveDrab;
	static Color Orange;
	static Color OrangeRed;
	static Color Orchid;
	static Color PaleGoldenrod;
	static Color PaleGreen;
	static Color PaleTurquoise;
	static Color PaleVioletRed;
	static Color PapayaWhip;
	static Color PeachPuff;
	static Color Peru;
	static Color Pink;
	static Color Plum;
	static Color PowderBlue;
	static Color Purple;
	static Color Red;
	static Color RosyBrown;
	static Color RoyalBlue;
	static Color SaddleBrown;
	static Color Salmon;
	static Color SandyBrown;
	static Color SeaGreen;
	static Color SeaShell;
	static Color Sienna;
	static Color Silver;
	static Color SkyBlue;
	static Color SlateBlue;
	static Color SlateGray;
	static Color Snow;
	static Color SpringGreen;
	static Color SteelBlue;
	static Color Tan;
	static Color Teal;
	static Color Thistle;
	static Color Tomato;
	static Color Transparent;
	static Color Turquoise;
	static Color Violet;
	static Color Wheat;
	static Color White;
	static Color WhiteSmoke;
	static Color Yellow;
	static Color YellowGreen;
};
#pragma pack(pop)

CORE_API Color operator +(double left, const Color& right);
CORE_API Color operator -(double left, const Color& right);
CORE_API Color operator *(double left, const Color& right);
CORE_API Color operator /(double left, const Color& right);

#include "Color.inl"