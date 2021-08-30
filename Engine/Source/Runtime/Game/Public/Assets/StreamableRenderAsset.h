// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "Asset.h"

class GAME_API StreamableRenderAsset : public Asset
{
	CLASS_BODY(StreamableRenderAsset)

public:
	StreamableRenderAsset(const std::filesystem::path& assetPath);
	~StreamableRenderAsset() override;

	virtual void StreamIn() = 0;
};