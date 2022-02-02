// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SAssetsLoader;

class GAME_API SStreamableRenderAsset : implements SObject
{
	GENERATED_BODY(SStreamableRenderAsset)

public:
	SStreamableRenderAsset();

	virtual bool StreamIn(SAssetsLoader* Loader, std::span<const uint8> AssetsBin) = 0;
};