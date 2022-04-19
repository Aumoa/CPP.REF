// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Components/SceneComponent.h"
#include "MinimalViewInfo.h"

namespace libty::inline Game
{
	class GAME_API SCameraComponent : extends(SSceneComponent)
	{
		GENERATED_BODY(SCameraComponent);

	public:
		SCameraComponent();

		MinimalViewInfo GetViewInfo(float elapsedTime);
	};
}