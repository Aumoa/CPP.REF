// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Info.h"

namespace libty::inline Game
{
	class APlayerController;

	class GAME_API AGameMode : extends(AInfo)
	{
		GENERATED_BODY(AGameMode);

	public:
		SubclassOf<APlayerController> PlayerControllerClass;

	public:
		AGameMode();

		APlayerController* SpawnPlayerController();
	};
}