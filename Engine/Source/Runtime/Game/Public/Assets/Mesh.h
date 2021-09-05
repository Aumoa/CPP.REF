// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class GAME_API Mesh : public StreamableRenderAsset
{
	GENERATED_BODY(Mesh)

public:
	Mesh(const std::filesystem::path& assetPath);
	~Mesh() override;
};