// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <string_view>
#include "Vector.h"
#include "Mathematics/MathEx.h"

namespace libty::inline CoreNumerics
{
	struct Color
	{
		float R;
		float G;
		float B;
		float A;

	public:
		constexpr Color(float R, float G, float B, float A = 1.0f) : R(R), G(G), B(B), A(A)
		{
		}

		constexpr Color(const Color& Rhs) : R(Rhs.R), G(Rhs.G), B(Rhs.B), A(Rhs.A)
		{
		}

		constexpr Color(const Color& Rhs, float A) : R(Rhs.R), G(Rhs.G), B(Rhs.B), A(A)
		{
		}

		template<TIsVector<float, 4> IVector>
		constexpr Color(const IVector& V) : R(V[0]), G(V[1]), B(V[2]), A(V[3])
		{
		}

		constexpr Color& operator =(const Color& C)
		{
			R = C.R;
			G = C.G;
			B = C.B;
			A = C.A;
			return *this;
		}

	public:
		using Type = float;

		constexpr Color(float S = 0) : R(S), G(S), B(S), A(S)
		{
		}

		inline static constexpr size_t Size()
		{
			return 4;
		}

		inline constexpr Color operator -() const
		{
			return Color(-R, -G, -B, -A);
		}

		inline constexpr const float& operator [](size_t N) const
		{
			switch (N)
			{
			case 0:
				return R;
			case 1:
				return G;
			case 2:
				return B;
			case 3:
			default:
				return A;
			}
		}

		inline constexpr float& operator [](size_t N)
		{
			switch (N)
			{
			case 0:
				return R;
			case 1:
				return G;
			case 2:
				return B;
			case 3:
			default:
				return A;
			}
		}

		template<TIsVector<float, 4> IVector>
		constexpr inline Color& operator =(const IVector& V)
		{
			R = V[0];
			G = V[1];
			B = V[2];
			A = V[3];
			return *this;
		}

	public:
		inline constexpr Vector<uint8, 4> AsBytes() const
		{
			return Vector<uint8, 4>
			{
				(uint8)((R * 255.0f) + 0.5f),
				(uint8)((G * 255.0f) + 0.5f),
				(uint8)((B * 255.0f) + 0.5f),
				(uint8)((A * 255.0f) + 0.5f)
			};
		}

		inline std::wstring ToString() const
		{
			return String::Format(L"#{:0>8X}", ToUInt());
		}

		template<TIsVector<float, 4> IVectorL, TIsVector<float, 4> IVectorR>
		static constexpr Color AlphaBlend(const IVectorL& VL, const IVectorR& VR, float A = 1.0f)
		{
			Color R = VL * (1.0f - A) + VR * A;
			R.A = 1.0f;
			return R;
		}

		template<TIsVector<float, 4> IVector>
		inline constexpr Color operator ^(const IVector& V)
		{
			return AlphaBlend(*this, V);
		}

		inline constexpr uint32 ToUInt() const
		{
			Vector<uint8, 4> Bytes = AsBytes();
			return
				(((uint32)(Bytes[0]) & 0xFF) << 24) |
				(((uint32)(Bytes[1]) & 0xFF) << 16) |
				(((uint32)(Bytes[2]) & 0xFF) << 8) |
				(((uint32)(Bytes[3]) & 0xFF) << 0);
		}

		inline static constexpr Color FromUInt(uint32 ARGB)
		{
			return FromBytes((uint8)(ARGB >> 16), (uint8)(ARGB >> 8), (uint8)ARGB, (uint8)(ARGB >> 24));
		}

		inline static constexpr Color FromBytes(uint8 R, uint8 G, uint8 B, uint8 A)
		{
			constexpr float Inv255 = 1.0f / 255.0f;
			return Color((float)R * Inv255, (float)G * Inv255, (float)B * Inv255, (float)A * Inv255);
		}

		CORENUMERICS_API static Color FromHtml(std::wstring_view HtmlCode);
	};

	namespace NamedColors
	{
		constexpr Color AliceBlue = Color::FromUInt(0xFFF0F8FF);
		constexpr Color AntiqueWhite = Color::FromUInt(0xFFFAEBD7);
		constexpr Color Aqua = Color::FromUInt(0xFF00FFFF);
		constexpr Color Aquamarine = Color::FromUInt(0xFF7FFFD4);
		constexpr Color Azure = Color::FromUInt(0xFFF0FFFF);
		constexpr Color Beige = Color::FromUInt(0xFFF5F5DC);
		constexpr Color Bisque = Color::FromUInt(0xFFFFE4C4);
		constexpr Color Black = Color::FromUInt(0xFF000000);
		constexpr Color BlanchedAlmond = Color::FromUInt(0xFFFFEBCD);
		constexpr Color Blue = Color::FromUInt(0xFF0000FF);
		constexpr Color BlueViolet = Color::FromUInt(0xFF8A2BE2);
		constexpr Color Brown = Color::FromUInt(0xFFA52A2A);
		constexpr Color BurlyWood = Color::FromUInt(0xFFDEB887);
		constexpr Color CadetBlue = Color::FromUInt(0xFF5F9EA0);
		constexpr Color Chartreuse = Color::FromUInt(0xFF7FFF00);
		constexpr Color Chocolate = Color::FromUInt(0xFFD2691E);
		constexpr Color Coral = Color::FromUInt(0xFFFF7F50);
		constexpr Color CornflowerBlue = Color::FromUInt(0xFF6495ED);
		constexpr Color Cornsilk = Color::FromUInt(0xFFFFF8DC);
		constexpr Color Crimson = Color::FromUInt(0xFFDC143C);
		constexpr Color Cyan = Color::FromUInt(0xFF00FFFF);
		constexpr Color DarkBlue = Color::FromUInt(0xFF00008B);
		constexpr Color DarkCyan = Color::FromUInt(0xFF008B8B);
		constexpr Color DarkGoldenrod = Color::FromUInt(0xFFB8860B);
		constexpr Color DarkGray = Color::FromUInt(0xFFA9A9A9);
		constexpr Color DarkGreen = Color::FromUInt(0xFF006400);
		constexpr Color DarkKhaki = Color::FromUInt(0xFFBDB76B);
		constexpr Color DarkMagenta = Color::FromUInt(0xFF8B008B);
		constexpr Color DarkOliveGreen = Color::FromUInt(0xFF556B2F);
		constexpr Color DarkOrange = Color::FromUInt(0xFFFF8C00);
		constexpr Color DarkOrchid = Color::FromUInt(0xFF9932CC);
		constexpr Color DarkRed = Color::FromUInt(0xFF8B0000);
		constexpr Color DarkSalmon = Color::FromUInt(0xFFE9967A);
		constexpr Color DarkSeaGreen = Color::FromUInt(0xFF8FBC8B);
		constexpr Color DarkSlateBlue = Color::FromUInt(0xFF483D8B);
		constexpr Color DarkSlateGray = Color::FromUInt(0xFF2F4F4F);
		constexpr Color DarkTurquoise = Color::FromUInt(0xFF00CED1);
		constexpr Color DarkViolet = Color::FromUInt(0xFF9400D3);
		constexpr Color DeepPink = Color::FromUInt(0xFFFF1493);
		constexpr Color DeepSkyBlue = Color::FromUInt(0xFF00BFFF);
		constexpr Color DimGray = Color::FromUInt(0xFF696969);
		constexpr Color DodgerBlue = Color::FromUInt(0xFF1E90FF);
		constexpr Color Firebrick = Color::FromUInt(0xFFB22222);
		constexpr Color FloralWhite = Color::FromUInt(0xFFFFFAF0);
		constexpr Color ForestGreen = Color::FromUInt(0xFF228B22);
		constexpr Color Fuchsia = Color::FromUInt(0xFFFF00FF);
		constexpr Color Gainsboro = Color::FromUInt(0xFFDCDCDC);
		constexpr Color GhostWhite = Color::FromUInt(0xFFF8F8FF);
		constexpr Color Gold = Color::FromUInt(0xFFFFD700);
		constexpr Color Goldenrod = Color::FromUInt(0xFFDAA520);
		constexpr Color Gray = Color::FromUInt(0xFF808080);
		constexpr Color Green = Color::FromUInt(0xFF008000);
		constexpr Color GreenYellow = Color::FromUInt(0xFFADFF2F);
		constexpr Color Honeydew = Color::FromUInt(0xFFF0FFF0);
		constexpr Color HotPink = Color::FromUInt(0xFFFF69B4);
		constexpr Color IndianRed = Color::FromUInt(0xFFCD5C5C);
		constexpr Color Indigo = Color::FromUInt(0xFF4B0082);
		constexpr Color Ivory = Color::FromUInt(0xFFFFFFF0);
		constexpr Color Khaki = Color::FromUInt(0xFFF0E68C);
		constexpr Color Lavender = Color::FromUInt(0xFFE6E6FA);
		constexpr Color LavenderBlush = Color::FromUInt(0xFFFFF0F5);
		constexpr Color LawnGreen = Color::FromUInt(0xFF7CFC00);
		constexpr Color LemonChiffon = Color::FromUInt(0xFFFFFACD);
		constexpr Color LightBlue = Color::FromUInt(0xFFADD8E6);
		constexpr Color LightCoral = Color::FromUInt(0xFFF08080);
		constexpr Color LightCyan = Color::FromUInt(0xFFE0FFFF);
		constexpr Color LightGoldenrodYellow = Color::FromUInt(0xFFFAFAD2);
		constexpr Color LightGray = Color::FromUInt(0xFFD3D3D3);
		constexpr Color LightGreen = Color::FromUInt(0xFF90EE90);
		constexpr Color LightPink = Color::FromUInt(0xFFFFB6C1);
		constexpr Color LightSalmon = Color::FromUInt(0xFFFFA07A);
		constexpr Color LightSeaGreen = Color::FromUInt(0xFF20B2AA);
		constexpr Color LightSkyBlue = Color::FromUInt(0xFF87CEFA);
		constexpr Color LightSlateGray = Color::FromUInt(0xFF778899);
		constexpr Color LightSteelBlue = Color::FromUInt(0xFFB0C4DE);
		constexpr Color LightYellow = Color::FromUInt(0xFFFFFFE0);
		constexpr Color Lime = Color::FromUInt(0xFF00FF00);
		constexpr Color LimeGreen = Color::FromUInt(0xFF32CD32);
		constexpr Color Linen = Color::FromUInt(0xFFFAF0E6);
		constexpr Color Magenta = Color::FromUInt(0xFFFF00FF);
		constexpr Color Maroon = Color::FromUInt(0xFF800000);
		constexpr Color MediumAquamarine = Color::FromUInt(0xFF66CDAA);
		constexpr Color MediumBlue = Color::FromUInt(0xFF0000CD);
		constexpr Color MediumOrchid = Color::FromUInt(0xFFBA55D3);
		constexpr Color MediumPurple = Color::FromUInt(0xFF9370DB);
		constexpr Color MediumSeaGreen = Color::FromUInt(0xFF3CB371);
		constexpr Color MediumSlateBlue = Color::FromUInt(0xFF7B68EE);
		constexpr Color MediumSpringGreen = Color::FromUInt(0xFF00FA9A);
		constexpr Color MediumTurquoise = Color::FromUInt(0xFF48D1CC);
		constexpr Color MediumVioletRed = Color::FromUInt(0xFFC71585);
		constexpr Color MidnightBlue = Color::FromUInt(0xFF191970);
		constexpr Color MintCream = Color::FromUInt(0xFFF5FFFA);
		constexpr Color MistyRose = Color::FromUInt(0xFFFFE4E1);
		constexpr Color Moccasin = Color::FromUInt(0xFFFFE4B5);
		constexpr Color NavajoWhite = Color::FromUInt(0xFFFFDEAD);
		constexpr Color Navy = Color::FromUInt(0xFF000080);
		constexpr Color OldLace = Color::FromUInt(0xFFFDF5E6);
		constexpr Color Olive = Color::FromUInt(0xFF808000);
		constexpr Color OliveDrab = Color::FromUInt(0xFF6B8E23);
		constexpr Color Orange = Color::FromUInt(0xFFFFA500);
		constexpr Color OrangeRed = Color::FromUInt(0xFFFF4500);
		constexpr Color Orchid = Color::FromUInt(0xFFDA70D6);
		constexpr Color PaleGoldenrod = Color::FromUInt(0xFFEEE8AA);
		constexpr Color PaleGreen = Color::FromUInt(0xFF98FB98);
		constexpr Color PaleTurquoise = Color::FromUInt(0xFFAFEEEE);
		constexpr Color PaleVioletRed = Color::FromUInt(0xFFDB7093);
		constexpr Color PapayaWhip = Color::FromUInt(0xFFFFEFD5);
		constexpr Color PeachPuff = Color::FromUInt(0xFFFFDAB9);
		constexpr Color Peru = Color::FromUInt(0xFFCD853F);
		constexpr Color Pink = Color::FromUInt(0xFFFFC0CB);
		constexpr Color Plum = Color::FromUInt(0xFFDDA0DD);
		constexpr Color PowderBlue = Color::FromUInt(0xFFB0E0E6);
		constexpr Color Purple = Color::FromUInt(0xFF800080);
		constexpr Color Red = Color::FromUInt(0xFFFF0000);
		constexpr Color RosyBrown = Color::FromUInt(0xFFBC8F8F);
		constexpr Color RoyalBlue = Color::FromUInt(0xFF4169E1);
		constexpr Color SaddleBrown = Color::FromUInt(0xFF8B4513);
		constexpr Color Salmon = Color::FromUInt(0xFFFA8072);
		constexpr Color SandyBrown = Color::FromUInt(0xFFF4A460);
		constexpr Color SeaGreen = Color::FromUInt(0xFF2E8B57);
		constexpr Color SeaShell = Color::FromUInt(0xFFFFF5EE);
		constexpr Color Sienna = Color::FromUInt(0xFFA0522D);
		constexpr Color Silver = Color::FromUInt(0xFFC0C0C0);
		constexpr Color SkyBlue = Color::FromUInt(0xFF87CEEB);
		constexpr Color SlateBlue = Color::FromUInt(0xFF6A5ACD);
		constexpr Color SlateGray = Color::FromUInt(0xFF708090);
		constexpr Color Snow = Color::FromUInt(0xFFFFFAFA);
		constexpr Color SpringGreen = Color::FromUInt(0xFF00FF7F);
		constexpr Color SteelBlue = Color::FromUInt(0xFF4682B4);
		constexpr Color Tan = Color::FromUInt(0xFFD2B48C);
		constexpr Color Teal = Color::FromUInt(0xFF008080);
		constexpr Color Thistle = Color::FromUInt(0xFFD8BFD8);
		constexpr Color Tomato = Color::FromUInt(0xFFFF6347);
		constexpr Color Transparent = Color::FromUInt(0x00000000);
		constexpr Color Turquoise = Color::FromUInt(0xFF40E0D0);
		constexpr Color Violet = Color::FromUInt(0xFFEE82EE);
		constexpr Color Wheat = Color::FromUInt(0xFFF5DEB3);
		constexpr Color White = Color::FromUInt(0xFFFFFFFF);
		constexpr Color WhiteSmoke = Color::FromUInt(0xFFF5F5F5);
		constexpr Color Yellow = Color::FromUInt(0xFFFFFF00);
		constexpr Color YellowGreen = Color::FromUInt(0xFF9ACD32);
	}
}