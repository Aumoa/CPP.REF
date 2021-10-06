// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "StreamableRenderAsset.h"

interface IRHITexture2D;

class GAME_API STexture2D : public SStreamableRenderAsset
{
	GENERATED_BODY(STexture2D)

private:
	IRHITexture2D* _texture = nullptr;

public:
	STexture2D(const std::filesystem::path& assetPath);
	virtual ~STexture2D() override;

	virtual void StreamIn() override;

	IRHITexture2D* GetRHITexture() { return _texture; }
	Vector2N GetPixelSize();
};