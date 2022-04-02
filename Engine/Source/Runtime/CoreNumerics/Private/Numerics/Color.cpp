// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Numerics/VectorInterface/Color.h"
#include <map>

using namespace libty;

static std::map<std::wstring_view, Color> gConvertTable =
{
	{ L"AliceBlue", NamedColors::AliceBlue },
	{ L"AntiqueWhite", NamedColors::AntiqueWhite },
	{ L"Aqua", NamedColors::Aqua },
	{ L"Aquamarine", NamedColors::Aquamarine },
	{ L"Azure", NamedColors::Azure },
	{ L"Beige", NamedColors::Beige },
	{ L"Bisque", NamedColors::Bisque },
	{ L"Black", NamedColors::Black },
	{ L"BlanchedAlmond", NamedColors::BlanchedAlmond },
	{ L"Blue", NamedColors::Blue },
	{ L"BlueViolet", NamedColors::BlueViolet },
	{ L"Brown", NamedColors::Brown },
	{ L"BurlyWood", NamedColors::BurlyWood },
	{ L"CadetBlue", NamedColors::CadetBlue },
	{ L"Chartreuse", NamedColors::Chartreuse },
	{ L"Chocolate", NamedColors::Chocolate },
	{ L"Coral", NamedColors::Coral },
	{ L"CornflowerBlue", NamedColors::CornflowerBlue },
	{ L"Cornsilk", NamedColors::Cornsilk },
	{ L"Crimson", NamedColors::Crimson },
	{ L"Cyan", NamedColors::Cyan },
	{ L"DarkBlue", NamedColors::DarkBlue },
	{ L"DarkCyan", NamedColors::DarkCyan },
	{ L"DarkGoldenrod", NamedColors::DarkGoldenrod },
	{ L"DarkGray", NamedColors::DarkGray },
	{ L"DarkGreen", NamedColors::DarkGreen },
	{ L"DarkKhaki", NamedColors::DarkKhaki },
	{ L"DarkMagenta", NamedColors::DarkMagenta },
	{ L"DarkOliveGreen", NamedColors::DarkOliveGreen },
	{ L"DarkOrange", NamedColors::DarkOrange },
	{ L"DarkOrchid", NamedColors::DarkOrchid },
	{ L"DarkRed", NamedColors::DarkRed },
	{ L"DarkSalmon", NamedColors::DarkSalmon },
	{ L"DarkSeaGreen", NamedColors::DarkSeaGreen },
	{ L"DarkSlateBlue", NamedColors::DarkSlateBlue },
	{ L"DarkSlateGray", NamedColors::DarkSlateGray },
	{ L"DarkTurquoise", NamedColors::DarkTurquoise },
	{ L"DarkViolet", NamedColors::DarkViolet },
	{ L"DeepPink", NamedColors::DeepPink },
	{ L"DeepSkyBlue", NamedColors::DeepSkyBlue },
	{ L"DimGray", NamedColors::DimGray },
	{ L"DodgerBlue", NamedColors::DodgerBlue },
	{ L"Firebrick", NamedColors::Firebrick },
	{ L"FloralWhite", NamedColors::FloralWhite },
	{ L"ForestGreen", NamedColors::ForestGreen },
	{ L"Fuchsia", NamedColors::Fuchsia },
	{ L"Gainsboro", NamedColors::Gainsboro },
	{ L"GhostWhite", NamedColors::GhostWhite },
	{ L"Gold", NamedColors::Gold },
	{ L"Goldenrod", NamedColors::Goldenrod },
	{ L"Gray", NamedColors::Gray },
	{ L"Green", NamedColors::Green },
	{ L"GreenYellow", NamedColors::GreenYellow },
	{ L"Honeydew", NamedColors::Honeydew },
	{ L"HotPink", NamedColors::HotPink },
	{ L"IndianRed", NamedColors::IndianRed },
	{ L"Indigo", NamedColors::Indigo },
	{ L"Ivory", NamedColors::Ivory },
	{ L"Khaki", NamedColors::Khaki },
	{ L"Lavender", NamedColors::Lavender },
	{ L"LavenderBlush", NamedColors::LavenderBlush },
	{ L"LawnGreen", NamedColors::LawnGreen },
	{ L"LemonChiffon", NamedColors::LemonChiffon },
	{ L"LightBlue", NamedColors::LightBlue },
	{ L"LightCoral", NamedColors::LightCoral },
	{ L"LightCyan", NamedColors::LightCyan },
	{ L"LightGoldenrodYellow", NamedColors::LightGoldenrodYellow },
	{ L"LightGray", NamedColors::LightGray },
	{ L"LightGreen", NamedColors::LightGreen },
	{ L"LightPink", NamedColors::LightPink },
	{ L"LightSalmon", NamedColors::LightSalmon },
	{ L"LightSeaGreen", NamedColors::LightSeaGreen },
	{ L"LightSkyBlue", NamedColors::LightSkyBlue },
	{ L"LightSlateGray", NamedColors::LightSlateGray },
	{ L"LightSteelBlue", NamedColors::LightSteelBlue },
	{ L"LightYellow", NamedColors::LightYellow },
	{ L"Lime", NamedColors::Lime },
	{ L"LimeGreen", NamedColors::LimeGreen },
	{ L"Linen", NamedColors::Linen },
	{ L"Magenta", NamedColors::Magenta },
	{ L"Maroon", NamedColors::Maroon },
	{ L"MediumAquamarine", NamedColors::MediumAquamarine },
	{ L"MediumBlue", NamedColors::MediumBlue },
	{ L"MediumOrchid", NamedColors::MediumOrchid },
	{ L"MediumPurple", NamedColors::MediumPurple },
	{ L"MediumSeaGreen", NamedColors::MediumSeaGreen },
	{ L"MediumSlateBlue", NamedColors::MediumSlateBlue },
	{ L"MediumSpringGreen", NamedColors::MediumSpringGreen },
	{ L"MediumTurquoise", NamedColors::MediumTurquoise },
	{ L"MediumVioletRed", NamedColors::MediumVioletRed },
	{ L"MidnightBlue", NamedColors::MidnightBlue },
	{ L"MintCream", NamedColors::MintCream },
	{ L"MistyRose", NamedColors::MistyRose },
	{ L"Moccasin", NamedColors::Moccasin },
	{ L"NavajoWhite", NamedColors::NavajoWhite },
	{ L"Navy", NamedColors::Navy },
	{ L"OldLace", NamedColors::OldLace },
	{ L"Olive", NamedColors::Olive },
	{ L"OliveDrab", NamedColors::OliveDrab },
	{ L"Orange", NamedColors::Orange },
	{ L"OrangeRed", NamedColors::OrangeRed },
	{ L"Orchid", NamedColors::Orchid },
	{ L"PaleGoldenrod", NamedColors::PaleGoldenrod },
	{ L"PaleGreen", NamedColors::PaleGreen },
	{ L"PaleTurquoise", NamedColors::PaleTurquoise },
	{ L"PaleVioletRed", NamedColors::PaleVioletRed },
	{ L"PapayaWhip", NamedColors::PapayaWhip },
	{ L"PeachPuff", NamedColors::PeachPuff },
	{ L"Peru", NamedColors::Peru },
	{ L"Pink", NamedColors::Pink },
	{ L"Plum", NamedColors::Plum },
	{ L"PowderBlue", NamedColors::PowderBlue },
	{ L"Purple", NamedColors::Purple },
	{ L"Red", NamedColors::Red },
	{ L"RosyBrown", NamedColors::RosyBrown },
	{ L"RoyalBlue", NamedColors::RoyalBlue },
	{ L"SaddleBrown", NamedColors::SaddleBrown },
	{ L"Salmon", NamedColors::Salmon },
	{ L"SandyBrown", NamedColors::SandyBrown },
	{ L"SeaGreen", NamedColors::SeaGreen },
	{ L"SeaShell", NamedColors::SeaShell },
	{ L"Sienna", NamedColors::Sienna },
	{ L"Silver", NamedColors::Silver },
	{ L"SkyBlue", NamedColors::SkyBlue },
	{ L"SlateBlue", NamedColors::SlateBlue },
	{ L"SlateGray", NamedColors::SlateGray },
	{ L"Snow", NamedColors::Snow },
	{ L"SpringGreen", NamedColors::SpringGreen },
	{ L"SteelBlue", NamedColors::SteelBlue },
	{ L"Tan", NamedColors::Tan },
	{ L"Teal", NamedColors::Teal },
	{ L"Thistle", NamedColors::Thistle },
	{ L"Tomato", NamedColors::Tomato },
	{ L"Transparent", NamedColors::Transparent },
	{ L"Turquoise", NamedColors::Turquoise },
	{ L"Violet", NamedColors::Violet },
	{ L"Wheat", NamedColors::Wheat },
	{ L"White", NamedColors::White },
	{ L"WhiteSmoke", NamedColors::WhiteSmoke },
	{ L"Yellow", NamedColors::Yellow },
	{ L"YellowGreen", NamedColors::YellowGreen },
};

Color Color::FromHtml(std::wstring_view HtmlCode)
{
	if (HtmlCode == L"")
	{
		return NamedColors::Transparent;
	}

	// #AARRGGBB
	if (HtmlCode.length() == 9)
	{
		std::wistringstream wiss;
		wiss.str(std::wstring(HtmlCode.substr(1, 8)));
		uint32 hexCode;
		wiss >> std::hex >> hexCode;
		return Color::FromUInt(hexCode);
	}

	// #RRGGBB
	if (HtmlCode.length() == 7)
	{
		std::wistringstream wiss;
		wiss.str(std::wstring(HtmlCode.substr(1, 6)));
		uint32 hexCode;
		wiss >> std::hex >> hexCode;
		hexCode |= 0xFF000000;
		return Color::FromUInt(hexCode);
	}

	// In table color.
	auto it = gConvertTable.find(HtmlCode);
	if (it != gConvertTable.end())
	{
		return it->second;
	}

	return NamedColors::Transparent;
}