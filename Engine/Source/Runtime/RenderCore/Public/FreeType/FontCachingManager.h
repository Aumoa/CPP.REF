// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "FreeTypeStructures.h"

class SRHIDevice;
class SFontFace;
class SFontFaceCachingNode;
class SRHIShaderResourceView;

class RENDERCORE_API SFontCachingManager : implements SObject
{
	GENERATED_BODY(SFontCachingManager)

private:
	SRHIDevice* _device = nullptr;
	std::map<std::wstring, SFontFaceCachingNode*> _nodes;

public:
	SFontCachingManager(SRHIDevice* device);
	virtual ~SFontCachingManager() override;

	void StreamGlyphs(SFontFace* face, std::wstring_view glyphs);
	void Apply();

	SRHIShaderResourceView* GetFontFaceRenderingView(SFontFace* face) const;
	std::vector<GlyphRenderInfo> QueryGlyphsRenderInfo(SFontFace* face, int32 fontSize, std::wstring_view text) const;
};