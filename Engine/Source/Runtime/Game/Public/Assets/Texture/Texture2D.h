// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/StreamableRenderAsset.h"

struct IPlatformImage;
struct IRHITexture2D;
struct IRHIBitmap;

class GAME_API STexture2D : public SStreamableRenderAsset
{
	GENERATED_BODY(STexture2D)

private:
	static constexpr uint32 ImporterVersion = 1;

	IPlatformImage* ImageSource = nullptr;
	IRHITexture2D* Texture = nullptr;
	IRHIBitmap* Bitmap = nullptr;

public:
	STexture2D();

	virtual bool StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin) override;
	static std::vector<uint8> StreamOut(SAssetsLoader* Loader, std::span<const uint8> AssetsBin);

	Vector2N GetSize();

	IPlatformImage* GetPlatformImage();
	IRHITexture2D* GetTexture();
	IRHIBitmap* GetBitmap();
};