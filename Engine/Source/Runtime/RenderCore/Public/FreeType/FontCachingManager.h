// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class RHIDevice;
class FontFace;
class FontFaceCachingNode;

class RENDERCORE_API FontCachingManager : virtual public Object
{
	GENERATED_BODY(FontCachingManager)

private:
	RHIDevice* _device = nullptr;
	std::map<std::wstring, FontFaceCachingNode*> _nodes;

public:
	FontCachingManager(RHIDevice* device);
	virtual ~FontCachingManager() override;

	void StreamGlyphs(FontFace* face, std::wstring_view glyphs);
	void Apply();

	class RHIShaderResourceView* GetDebugTexture() const;
};