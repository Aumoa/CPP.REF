// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "StreamableRenderAsset.h"

interface IRHITexture2D;
interface IRHIBitmap;

class GAME_API STexture2D : public SStreamableRenderAsset
{
	GENERATED_BODY(STexture2D)

private:
	IRHITexture2D* Texture = nullptr;
	IRHIBitmap* Bitmap = nullptr;

public:
	STexture2D(const std::filesystem::path& assetPath);
	virtual ~STexture2D() override;

	virtual void StreamIn() override;

	IRHITexture2D* GetRHITexture() { return Texture; }
	IRHIBitmap* GetRHIBitmap() { return Bitmap; }
	Vector2N GetPixelSize();
};