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

		virtual void Init() override;
		virtual void Deinit() override;

		void LoadGameModule(std::wstring_view GameModuleName);
		SGameInstance* LoadGameInstance();
	};
}