// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

namespace libty::inline Game
{
	class SGameInstance;

	class GAME_API SGameInstanceSubsystem : implements(SObject)
	{
		GENERATED_BODY(SGameInstanceSubsystem);

	private:
		SPROPERTY(GameInstance)
		SGameInstance* GameInstance = nullptr;

	public:
		SGameInstanceSubsystem();

		virtual void Init(SGameInstance* GameInstance);
		virtual void Deinit() {}

		SGameInstance* GetGameInstance();
	};
}