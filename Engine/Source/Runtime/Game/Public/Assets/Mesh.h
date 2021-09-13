// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class GAME_API SMesh : public SStreamableRenderAsset
{
	GENERATED_BODY(SMesh)

public:
	SMesh(const std::filesystem::path& assetPath);
	~SMesh() override;
};