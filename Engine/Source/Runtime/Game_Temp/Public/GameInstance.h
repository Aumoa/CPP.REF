// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameObject.h"

namespace libty::inline Game
{
	class SLevel;
	class SLocalPlayer;
	class SGameInstanceSubsystem;

	class GAME_API SGameInstance : public SGameObject
	{
		GENERATED_BODY(SGameInstance);

	public:
		SubclassOf<SLevel> StartupLevel;

	private:
		SPROPERTY(Subsystems)
		std::vector<SGameInstanceSubsystem*> Subsystems;

	public:
		SGameInstance();

		virtual void Init();

		SLocalPlayer* GetLocalPlayer();
		virtual SWorld* GetWorld() override;

		SGameInstanceSubsystem* GetSubsystem(SType* SubsystemClass, bool bAllowDerivedClass = true);
		template<std::derived_from<SGameInstanceSubsystem> T>
		T* GetSubsystem(bool bAllowDerivedClass = true)
		{
			return Cast<T>(GetSubsystem(T::StaticClass()));
		}

	private:
		void InitSubsystemCollection();
	};
}