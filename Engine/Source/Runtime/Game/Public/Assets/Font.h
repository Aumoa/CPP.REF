// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class SFontFace;

class GAME_API SFont : public SStreamableRenderAsset
{
	GENERATED_BODY(SFont)

private:
	SFontFace* _fontFace = nullptr;

public:
	SFont(const std::filesystem::path& assetPath);
	virtual ~SFont() override;

	virtual void StreamIn() override;

	inline SFontFace* GetFontFace() const { return _fontFace; }
};