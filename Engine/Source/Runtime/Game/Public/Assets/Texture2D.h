// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class RHITexture2D;

class GAME_API Texture2D : public StreamableRenderAsset
{
	CLASS_BODY(Texture2D)

private:
	RHITexture2D* _texture = nullptr;

public:
	Texture2D(const std::filesystem::path& assetPath);
	virtual ~Texture2D() override;

	virtual void StreamIn() override;
};