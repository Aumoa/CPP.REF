// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngineSubsystem.h"
#include "Level/WorldType.h"

namespace libty::inline Game
{
	class SLevel;
	class SWorld;

	class GAME_API SGameLevelSystem : extends(SGameEngineSubsystem)
	{
		GENERATED_BODY(SGameLevelSystem);

	private:
		SWorld* _GameWorld = nullptr;

	public:
		SGameLevelSystem();
		virtual ~SGameLevelSystem() override;

		virtual void PostInit() override;
		virtual void Deinit() override;

		SWorld* SpawnWorld(EWorldType InWorldType);

		SWorld* GetGameWorld() const;
		bool OpenLevel(SubclassOf<SLevel> InLevelToOpen);
	};
}