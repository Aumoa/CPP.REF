// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "FreeTypeStructures.h"

class SRHIDevice;
class SRHIDynamicTexture2D;
class SRHIShaderResourceView;
class SFontFace;

class RENDERCORE_API SFontFaceCachingNode : implements SObject
{
	GENERATED_BODY(SFontFaceCachingNode)

private:
	struct Glyph
	{
		wchar_t Character;
		int32 GlyphIndex;
		std::vector<uint8> Bitmap;
		size_t Hit = 0;
		Vector2N PixelSize;

		bool bLoad = false;
		int32 LocationX = 0;

		Vector2 LocalPosition;
		Vector2 LocalAdvance;
	};

	struct SizedGlyphCollection
	{
		int32 FontSize = 0;
		std::vector<Glyph> Glyphs;
		std::map<wchar_t, size_t> GlyphsView;
		int32 RequiredMaxWidth = 0;
		int32 RequiredMaxHeight = 0;

		bool bLoad = false;
		int32 LocationY = 0;
	};

private:
	SFontFace* _face = nullptr;
	std::vector<SizedGlyphCollection> _glyphCollections;
	std::map<int32, size_t> _glyphCollectionsView;
	int32 _requiredMaxWidth = 0;
	int32 _requiredMaxHeight = 0;

	SRHIDevice* _device = nullptr;
	SRHIDynamicTexture2D* _glyphBuffer = nullptr;
	SRHIShaderResourceView* _shaderResourceView = nullptr;
	bool _bNeedApply = false;

public:
	SFontFaceCachingNode(SFontFace* face, SRHIDevice* device);
	virtual ~SFontFaceCachingNode() override;

	void StreamGlyphs(std::wstring_view glyphs);
	void Apply();

	SRHIShaderResourceView* GetRenderingView() const;
	std::vector<GlyphRenderInfo> QueryGlyphsRenderInfo(int32 fontSize, std::wstring_view text) const;

private:
	bool ReallocateBufferIfRequired();

public:
	class SRHIShaderResourceView* GetDebugTexture() const;
};