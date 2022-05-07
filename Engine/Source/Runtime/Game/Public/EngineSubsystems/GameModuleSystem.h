// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngineSubsystem.h"

namespace libty::inline Game
{
	class SGameInstance;
	class SGameModule;

	class GAME_API SGameModuleSystem : public SGameEngineSubsystem
	{
		GENERATED_BODY(SGameModuleSystem)

	private:
		SPROPERTY(GameModule)
		SGameModule* GameModule = nullptr;

	public:
		SGameModuleSystem();
		virtual ~SGameModuleSystem() override;

		virtual Task<> StartAsync(std::stop_token CancellationToken) override;
		virtual Task<> StopAsync(std::stop_token CancellationToken) override;

		void LoadGameModule(std::wstring_view GameModuleName);
		SGameInstance* LoadGameInstance();
	};
}