// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"

class GAME_API SStreamableRenderAsset : public SAsset
{
	GENERATED_BODY(SStreamableRenderAsset)

public:
	SStreamableRenderAsset(const std::filesystem::path& assetPath);
	~SStreamableRenderAsset() override;

	virtual void StreamIn() = 0;
};