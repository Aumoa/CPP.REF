// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Player.h"

namespace libty::inline Game
{
	class APlayerController;

	class GAME_API SLocalPlayer : public SPlayer
	{
		GENERATED_BODY(SLocalPlayer);

	private:
		SPROPERTY(PlayerController)
		APlayerController* PlayerController = nullptr;

	public:
		SLocalPlayer();

		void SetPlayerController(APlayerController* InController);
		APlayerController* GetPlayerController();
	};
}