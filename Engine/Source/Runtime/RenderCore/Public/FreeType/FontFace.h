// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SFreeTypeModule;

class RENDERCORE_API SFontFace : virtual public SObject
{
	GENERATED_BODY(SFontFace)

public:
	struct Impl;
	
private:
	SFreeTypeModule* _module = nullptr;
	std::unique_ptr<Impl> _impl;
	const std::wstring _faceName;
	int32 _fontSize = 0;
	int32 _dpiScale = 0;
	int32 _glyphIndex = 0;
	std::optional<wchar_t> _loadedGlyph;

public:
	SFontFace(SFreeTypeModule* module, std::unique_ptr<Impl>&& impl, const std::wstring& faceName);
	virtual ~SFontFace() override;

	bool SetFontSize(int32 fontSize, int32 dpiScale = 96);
	bool LoadGlyph(wchar_t glyph);
	
	Vector2N GetGlyphPixelSize() const;
	bool CopyGlyphPixels(void* buffer, int32 rowStride, int32 locationX, int32 locationY) const;
	int32 GetMaxGlyphRenderHeight() const;
	Vector2 GetLocalPosition() const;
	Vector2 GetAdvance() const;
	int32 GetGlyphIndex() const;
	bool HasKerning() const;
	Vector2 GetKerning(int32 left, int32 right) const;

	inline SFreeTypeModule* GetModule() const { return _module; }
	inline int32 GetFontSize() const { return _fontSize; }
	inline int32 GetDPIScale() const { return _dpiScale; }
	inline std::wstring GetName() const { return _faceName; }
	inline bool IsGlyphLoaded() const { return _loadedGlyph.has_value(); }
};