// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "RenderMinimal.h"

class GAME_API StreamableRenderAsset : virtual public Object
{
	CLASS_BODY(StreamableRenderAsset)

public:
	StreamableRenderAsset();
	~StreamableRenderAsset() override;
};