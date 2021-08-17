// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"
#include "StreamableRenderAsset.h"

class GAME_API Mesh : public StreamableRenderAsset
{
public:
	using Super = StreamableRenderAsset;

public:
	Mesh();
	~Mesh() override;
};