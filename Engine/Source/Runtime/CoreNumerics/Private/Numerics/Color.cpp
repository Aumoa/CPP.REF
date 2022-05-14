// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Numerics/VectorInterface/Color.h"
#include <map>

using namespace libty;

static std::map<String, Color> gConvertTable =
{
	{ TEXT("AliceBlue"), NamedColors::AliceBlue },
	{ TEXT("AntiqueWhite"), NamedColors::AntiqueWhite },
	{ TEXT("Aqua"), NamedColors::Aqua },
	{ TEXT("Aquamarine"), NamedColors::Aquamarine },
	{ TEXT("Azure"), NamedColors::Azure },
	{ TEXT("Beige"), NamedColors::Beige },
	{ TEXT("Bisque"), NamedColors::Bisque },
	{ TEXT("Black"), NamedColors::Black },
	{ TEXT("BlanchedAlmond"), NamedColors::BlanchedAlmond },
	{ TEXT("Blue"), NamedColors::Blue },
	{ TEXT("BlueViolet"), NamedColors::BlueViolet },
	{ TEXT("Brown"), NamedColors::Brown },
	{ TEXT("BurlyWood"), NamedColors::BurlyWood },
	{ TEXT("CadetBlue"), NamedColors::CadetBlue },
	{ TEXT("Chartreuse"), NamedColors::Chartreuse },
	{ TEXT("Chocolate"), NamedColors::Chocolate },
	{ TEXT("Coral"), NamedColors::Coral },
	{ TEXT("CornflowerBlue"), NamedColors::CornflowerBlue },
	{ TEXT("Cornsilk"), NamedColors::Cornsilk },
	{ TEXT("Crimson"), NamedColors::Crimson },
	{ TEXT("Cyan"), NamedColors::Cyan },
	{ TEXT("DarkBlue"), NamedColors::DarkBlue },
	{ TEXT("DarkCyan"), NamedColors::DarkCyan },
	{ TEXT("DarkGoldenrod"), NamedColors::DarkGoldenrod },
	{ TEXT("DarkGray"), NamedColors::DarkGray },
	{ TEXT("DarkGreen"), NamedColors::DarkGreen },
	{ TEXT("DarkKhaki"), NamedColors::DarkKhaki },
	{ TEXT("DarkMagenta"), NamedColors::DarkMagenta },
	{ TEXT("DarkOliveGreen"), NamedColors::DarkOliveGreen },
	{ TEXT("DarkOrange"), NamedColors::DarkOrange },
	{ TEXT("DarkOrchid"), NamedColors::DarkOrchid },
	{ TEXT("DarkRed"), NamedColors::DarkRed },
	{ TEXT("DarkSalmon"), NamedColors::DarkSalmon },
	{ TEXT("DarkSeaGreen"), NamedColors::DarkSeaGreen },
	{ TEXT("DarkSlateBlue"), NamedColors::DarkSlateBlue },
	{ TEXT("DarkSlateGray"), NamedColors::DarkSlateGray },
	{ TEXT("DarkTurquoise"), NamedColors::DarkTurquoise },
	{ TEXT("DarkViolet"), NamedColors::DarkViolet },
	{ TEXT("DeepPink"), NamedColors::DeepPink },
	{ TEXT("DeepSkyBlue"), NamedColors::DeepSkyBlue },
	{ TEXT("DimGray"), NamedColors::DimGray },
	{ TEXT("DodgerBlue"), NamedColors::DodgerBlue },
	{ TEXT("Firebrick"), NamedColors::Firebrick },
	{ TEXT("FloralWhite"), NamedColors::FloralWhite },
	{ TEXT("ForestGreen"), NamedColors::ForestGreen },
	{ TEXT("Fuchsia"), NamedColors::Fuchsia },
	{ TEXT("Gainsboro"), NamedColors::Gainsboro },
	{ TEXT("GhostWhite"), NamedColors::GhostWhite },
	{ TEXT("Gold"), NamedColors::Gold },
	{ TEXT("Goldenrod"), NamedColors::Goldenrod },
	{ TEXT("Gray"), NamedColors::Gray },
	{ TEXT("Green"), NamedColors::Green },
	{ TEXT("GreenYellow"), NamedColors::GreenYellow },
	{ TEXT("Honeydew"), NamedColors::Honeydew },
	{ TEXT("HotPink"), NamedColors::HotPink },
	{ TEXT("IndianRed"), NamedColors::IndianRed },
	{ TEXT("Indigo"), NamedColors::Indigo },
	{ TEXT("Ivory"), NamedColors::Ivory },
	{ TEXT("Khaki"), NamedColors::Khaki },
	{ TEXT("Lavender"), NamedColors::Lavender },
	{ TEXT("LavenderBlush"), NamedColors::LavenderBlush },
	{ TEXT("LawnGreen"), NamedColors::LawnGreen },
	{ TEXT("LemonChiffon"), NamedColors::LemonChiffon },
	{ TEXT("LightBlue"), NamedColors::LightBlue },
	{ TEXT("LightCoral"), NamedColors::LightCoral },
	{ TEXT("LightCyan"), NamedColors::LightCyan },
	{ TEXT("LightGoldenrodYellow"), NamedColors::LightGoldenrodYellow },
	{ TEXT("LightGray"), NamedColors::LightGray },
	{ TEXT("LightGreen"), NamedColors::LightGreen },
	{ TEXT("LightPink"), NamedColors::LightPink },
	{ TEXT("LightSalmon"), NamedColors::LightSalmon },
	{ TEXT("LightSeaGreen"), NamedColors::LightSeaGreen },
	{ TEXT("LightSkyBlue"), NamedColors::LightSkyBlue },
	{ TEXT("LightSlateGray"), NamedColors::LightSlateGray },
	{ TEXT("LightSteelBlue"), NamedColors::LightSteelBlue },
	{ TEXT("LightYellow"), NamedColors::LightYellow },
	{ TEXT("Lime"), NamedColors::Lime },
	{ TEXT("LimeGreen"), NamedColors::LimeGreen },
	{ TEXT("Linen"), NamedColors::Linen },
	{ TEXT("Magenta"), NamedColors::Magenta },
	{ TEXT("Maroon"), NamedColors::Maroon },
	{ TEXT("MediumAquamarine"), NamedColors::MediumAquamarine },
	{ TEXT("MediumBlue"), NamedColors::MediumBlue },
	{ TEXT("MediumOrchid"), NamedColors::MediumOrchid },
	{ TEXT("MediumPurple"), NamedColors::MediumPurple },
	{ TEXT("MediumSeaGreen"), NamedColors::MediumSeaGreen },
	{ TEXT("MediumSlateBlue"), NamedColors::MediumSlateBlue },
	{ TEXT("MediumSpringGreen"), NamedColors::MediumSpringGreen },
	{ TEXT("MediumTurquoise"), NamedColors::MediumTurquoise },
	{ TEXT("MediumVioletRed"), NamedColors::MediumVioletRed },
	{ TEXT("MidnightBlue"), NamedColors::MidnightBlue },
	{ TEXT("MintCream"), NamedColors::MintCream },
	{ TEXT("MistyRose"), NamedColors::MistyRose },
	{ TEXT("Moccasin"), NamedColors::Moccasin },
	{ TEXT("NavajoWhite"), NamedColors::NavajoWhite },
	{ TEXT("Navy"), NamedColors::Navy },
	{ TEXT("OldLace"), NamedColors::OldLace },
	{ TEXT("Olive"), NamedColors::Olive },
	{ TEXT("OliveDrab"), NamedColors::OliveDrab },
	{ TEXT("Orange"), NamedColors::Orange },
	{ TEXT("OrangeRed"), NamedColors::OrangeRed },
	{ TEXT("Orchid"), NamedColors::Orchid },
	{ TEXT("PaleGoldenrod"), NamedColors::PaleGoldenrod },
	{ TEXT("PaleGreen"), NamedColors::PaleGreen },
	{ TEXT("PaleTurquoise"), NamedColors::PaleTurquoise },
	{ TEXT("PaleVioletRed"), NamedColors::PaleVioletRed },
	{ TEXT("PapayaWhip"), NamedColors::PapayaWhip },
	{ TEXT("PeachPuff"), NamedColors::PeachPuff },
	{ TEXT("Peru"), NamedColors::Peru },
	{ TEXT("Pink"), NamedColors::Pink },
	{ TEXT("Plum"), NamedColors::Plum },
	{ TEXT("PowderBlue"), NamedColors::PowderBlue },
	{ TEXT("Purple"), NamedColors::Purple },
	{ TEXT("Red"), NamedColors::Red },
	{ TEXT("RosyBrown"), NamedColors::RosyBrown },
	{ TEXT("RoyalBlue"), NamedColors::RoyalBlue },
	{ TEXT("SaddleBrown"), NamedColors::SaddleBrown },
	{ TEXT("Salmon"), NamedColors::Salmon },
	{ TEXT("SandyBrown"), NamedColors::SandyBrown },
	{ TEXT("SeaGreen"), NamedColors::SeaGreen },
	{ TEXT("SeaShell"), NamedColors::SeaShell },
	{ TEXT("Sienna"), NamedColors::Sienna },
	{ TEXT("Silver"), NamedColors::Silver },
	{ TEXT("SkyBlue"), NamedColors::SkyBlue },
	{ TEXT("SlateBlue"), NamedColors::SlateBlue },
	{ TEXT("SlateGray"), NamedColors::SlateGray },
	{ TEXT("Snow"), NamedColors::Snow },
	{ TEXT("SpringGreen"), NamedColors::SpringGreen },
	{ TEXT("SteelBlue"), NamedColors::SteelBlue },
	{ TEXT("Tan"), NamedColors::Tan },
	{ TEXT("Teal"), NamedColors::Teal },
	{ TEXT("Thistle"), NamedColors::Thistle },
	{ TEXT("Tomato"), NamedColors::Tomato },
	{ TEXT("Transparent"), NamedColors::Transparent },
	{ TEXT("Turquoise"), NamedColors::Turquoise },
	{ TEXT("Violet"), NamedColors::Violet },
	{ TEXT("Wheat"), NamedColors::Wheat },
	{ TEXT("White"), NamedColors::White },
	{ TEXT("WhiteSmoke"), NamedColors::WhiteSmoke },
	{ TEXT("Yellow"), NamedColors::Yellow },
	{ TEXT("YellowGreen"), NamedColors::YellowGreen },
};

Color Color::FromHtml(String HtmlCode)
{
	if (HtmlCode == TEXT(""))
	{
		return NamedColors::Transparent;
	}

	// #AARRGGBB
	if ((size_t)HtmlCode == 9)
	{
		std::wistringstream wiss;
		wiss.str((std::wstring)HtmlCode.Substring(1, 8));
		uint32 hexCode;
		wiss >> std::hex >> hexCode;
		return Color::FromUInt(hexCode);
	}

	// #RRGGBB
	if ((size_t)HtmlCode == 7)
	{
		std::wistringstream wiss;
		wiss.str(std::wstring(HtmlCode.Substring(1, 6)));
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