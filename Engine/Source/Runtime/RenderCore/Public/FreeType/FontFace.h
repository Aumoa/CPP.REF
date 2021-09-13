// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RENDERCORE_API SFontFace : virtual public SObject
{
	GENERATED_BODY(SFontFace)

public:
	struct Impl;
	
private:
	std::unique_ptr<Impl> _impl;
	const std::wstring _faceName;
	int32 _fontSize = 0;
	int32 _dpiScale = 0;
	std::optional<wchar_t> _loadedGlyph;

public:
	SFontFace(std::unique_ptr<Impl>&& impl, const std::wstring& faceName);
	virtual ~SFontFace() override;

	bool SetFontSize(int32 fontSize, int32 dpiScale = 96);
	bool LoadGlyph(wchar_t glyph);
	
	bool GetGlyphPixelSize(int32* pixelsX, int32* pixelsY) const;
	bool CopyGlyphPixels(void* buffer, int32 rowStride, int32 locationX, int32 locationY) const;
	int32 GetMaxGlyphRenderHeight() const;

	inline int32 GetFontSize() const { return _fontSize; }
	inline int32 GetDPIScale() const { return _dpiScale; }
	inline std::wstring GetName() const { return _faceName; }
	inline bool IsGlyphLoaded() const { return _loadedGlyph.has_value(); }
};