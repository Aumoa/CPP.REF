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
	for (int32 i = 0; i < (int32)bitmap.rows; ++i)
	{
		const int32 Y = i + locationY;

		const int32 StartDst = Y * rowStride + locationX;
		const int32 StartSrc = Y * bitmap.width;
		memcpy(buffer_view + StartDst, bitmap.buffer + StartSrc, CopyStride);
	}

	return true;
}

int32 FontFace::GetMaxGlyphRenderHeight() const
{
	return _impl->Face->size->metrics.height >> 6;
}