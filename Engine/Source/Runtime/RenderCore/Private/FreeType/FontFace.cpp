// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "FreeType/FontFace.h"
#include "FontFace.Impl.h"
#include "LogFreeType.h"

FontFace::FontFace(std::unique_ptr<Impl>&& impl, const std::wstring& faceName) : Super()
	, _impl(std::move(impl))
	, _faceName(faceName)
{
}

FontFace::~FontFace()
{
}

bool FontFace::SetFontSize(int32 fontSize, int32 dpiScale)
{
	ensure(dpiScale > 0);
	ensure(fontSize > 0);
	if (FT_Error error = FT_Set_Char_Size(_impl->Face, fontSize << 6, 0, (uint32)dpiScale, 0); error)
	{
		SE_LOG(LogFreeType, Error, L"Could not setting font size({}) and dpi scale({}) with error code: %d.", fontSize, dpiScale, error);
		return false;
	}

	_fontSize = fontSize;
	_dpiScale = dpiScale;
	return true;
}

bool FontFace::LoadGlyph(wchar_t glyph)
{
	ensure(glyph);
	_loadedGlyph.reset();

	uint32 glyphIndex = FT_Get_Char_Index(_impl->Face, (FT_ULong)glyph);
	if (glyphIndex == 0)
	{
		SE_LOG(LogFreeType, Error, L"Could not found glyph({}) index from font face({}).", glyph, _faceName);
		return false;
	}

	if (FT_Error error = FT_Load_Glyph(_impl->Face, glyphIndex, FT_LOAD_RENDER); error)
	{
		SE_LOG(LogFreeType, Error, L"Could not load glyph({}) with glyph index({}) from font face({}). ErrorCode: {}, Message: {}", glyph, glyphIndex, _faceName, error, ANSI_TO_WCHAR(FT_Error_String(error)));
		return false;
	}

	_loadedGlyph = glyph;
	auto r = _impl->Face->glyph->advance.x;
	return true;
}

bool FontFace::GetGlyphPixelSize(int32* pixelsX, int32* pixelsY) const
{
	if (!_loadedGlyph.has_value())
	{
		SE_LOG(LogFreeType, Error, L"Glyph does not loaded. See LoadGlyph function to load glyph.");
		return false;
	}

	auto& bitmap = _impl->Face->glyph->bitmap;
	if (pixelsX)
	{
		*pixelsX = (int32)bitmap.width;
	}
	if (pixelsY)
	{
		*pixelsY = (int32)bitmap.rows;
	}

	return true;
}

template<std::endian>
struct BitView
{
	uint8 bView7 : 1;
	uint8 bView6 : 1;
	uint8 bView5 : 1;
	uint8 bView4 : 1;
	uint8 bView3 : 1;
	uint8 bView2 : 1;
	uint8 bView1 : 1;
	uint8 bView0 : 1;
};

template<>
struct BitView<std::endian::big>
{
	uint8 bView7 : 1;
	uint8 bView6 : 1;
	uint8 bView5 : 1;
	uint8 bView4 : 1;
	uint8 bView3 : 1;
	uint8 bView2 : 1;
	uint8 bView1 : 1;
	uint8 bView0 : 1;
};

bool FontFace::CopyGlyphPixels(void* buffer, int32 rowStride, int32 locationX, int32 locationY) const
{
	if (!_loadedGlyph.has_value())
	{
		SE_LOG(LogFreeType, Error, L"Glyph does not loaded. See LoadGlyph function to load glyph.");
		return false;
	}

	auto& bitmap = _impl->Face->glyph->bitmap;

	int32 locationXEnd = locationX + bitmap.width;
	if (locationXEnd >= rowStride)
	{
		locationXEnd = rowStride;
	}

	int8* buffer_view = reinterpret_cast<int8*>(buffer);
	const int32 CopyStride = locationXEnd - locationX;

	if (bitmap.num_grays == 256)
	{
		for (int32 i = 0; i < (int32)bitmap.rows; ++i)
		{
			const int32 Y = i + locationY;

			const int32 StartDst = Y * rowStride + locationX;
			const int32 StartSrc = Y * bitmap.width;
			memcpy(buffer_view + StartDst, bitmap.buffer + StartSrc, CopyStride);
		}
	}
	else if (bitmap.num_grays == 2)
	{
		static auto set_value = [&bitmap, &buffer_view](int32 x, int32 index, uint8 value)
		{
			if (bitmap.width > (uint32)x)
			{
				buffer_view[index] = value;
				return true;
			}
			else
			{
				return false;
			}
		};

		for (int32 i = 0; i < (int32)bitmap.rows; ++i)
		{
			const int32 Y = i + locationY;

			const int32 StartDst = Y * rowStride + locationX;
			const int32 StartSrc = Y * bitmap.pitch;

			const int32 Iteration = (CopyStride - 1) / 8 + 1;
			for (int32 j = 0; j < Iteration; ++j)
			{
				const int32 IterationX = j * 8;

#define set_value_break(x) \
				if (!set_value(IterationX + x, StartDst + IterationX + x, bitView.bView ## x ? 255 : 0)) \
				{ \
					break; \
				}

				BitView<std::endian::native>& bitView = reinterpret_cast<BitView<std::endian::native>&>(bitmap.buffer[StartSrc + j]);
				set_value_break(0);
				set_value_break(1);
				set_value_break(2);
				set_value_break(3);
				set_value_break(4);
				set_value_break(5);
				set_value_break(6);
				set_value_break(7);

#undef set_value_break
			}
		}
	}
	else
	{
		check(false);
	}

	return true;
}

int32 FontFace::GetMaxGlyphRenderHeight() const
{
	return _impl->Face->size->metrics.height >> 6;
}