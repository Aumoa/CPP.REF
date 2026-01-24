// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Text/Encoding.h"
#include <array>

namespace Ayla
{
	constexpr std::array<char, 4> BOM_UTF32LE = { static_cast<char>(0xFF), static_cast<char>(0xFE), static_cast<char>(0x00), static_cast<char>(0x00) };
	constexpr std::array<char, 4> BOM_UTF32BE = { static_cast<char>(0x00), static_cast<char>(0x00), static_cast<char>(0xFE), static_cast<char>(0xFF) };
	constexpr std::array<char, 3> BOM_UTF8 = { static_cast<char>(0xEF), static_cast<char>(0xBB), static_cast<char>(0xBF) };
	constexpr std::array<char, 2> BOM_UTF16LE = { static_cast<char>(0xFF), static_cast<char>(0xFE) };
	constexpr std::array<char, 2> BOM_UTF16BE = { static_cast<char>(0xFE), static_cast<char>(0xFF) };

	template<size_t ArraySize>
	static bool BOMCheck(const std::array<char, ArraySize>& bom, const std::span<const char>& c_str, int32 codepage, int32* outCodepage, size_t* outOffset)
	{
		if (c_str.size() < ArraySize)
		{
			return false;
		}

		for (size_t i = 0; i < ArraySize; ++i)
		{
			if (c_str[i] != bom[i])
			{
				return false;
			}
		}

		*outOffset = ArraySize;
		*outCodepage = codepage;
		return true;
	}

	size_t Encoding::TryParseBOM(std::span<const char> c_str, int32* outCodepage)
	{
		size_t offset;
		*outCodepage = 0;
		if (BOMCheck(BOM_UTF32LE, c_str, 12000, outCodepage, &offset)) return offset;
		if (BOMCheck(BOM_UTF32BE, c_str, 12001, outCodepage, &offset)) return offset;
		if (BOMCheck(BOM_UTF8, c_str, 65001, outCodepage, &offset)) return offset;
		if (BOMCheck(BOM_UTF16LE, c_str, 1200, outCodepage, &offset)) return offset;
		if (BOMCheck(BOM_UTF16BE, c_str, 1201, outCodepage, &offset)) return offset;
		return 0;
	}
}