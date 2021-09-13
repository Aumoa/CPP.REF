// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SRHIDevice;
class SFontFace;
class SFontFaceCachingNode;

class RENDERCORE_API SFontCachingManager : virtual public SObject
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

	class SRHIShaderResourceView* GetDebugTexture() const;
};