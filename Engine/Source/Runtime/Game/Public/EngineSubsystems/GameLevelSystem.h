// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngineSubsystem.h"
#include "Level/WorldType.h"

namespace libty::inline Game
{
	class SLevel;
	class SWorld;

	class GAME_API SGameLevelSystem : public SGameEngineSubsystem
	{
		GENERATED_BODY(SGameLevelSystem);

	private:
		SPROPERTY(_GameWorld);
		SWorld* _GameWorld = nullptr;

	public:
		SGameLevelSystem();
		virtual ~SGameLevelSystem() override;

		virtual Task<> StartAsync(std::stop_token CancellationToken) override;
		virtual Task<> StopAsync(std::stop_token CancellationToken) override;

		SWorld* SpawnWorld(EWorldType InWorldType);

		SWorld* GetGameWorld() const;
		bool OpenLevel(SubclassOf<SLevel> InLevelToOpen);
	};
}