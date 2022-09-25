// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class GAME_API PrimitiveSceneInfo
{
	friend class RenderScene;

private:
	size_t PrimitiveId = -1;

public:
	PrimitiveSceneInfo();

	inline bool IsRegistered() const noexcept { return PrimitiveId != -1; }
	inline size_t GetPrimitiveId() const noexcept { return PrimitiveId; }
};