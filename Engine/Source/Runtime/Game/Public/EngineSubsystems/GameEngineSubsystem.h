// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "GameEngine.h"

namespace libty::inline Game
{
	class GAME_API SGameEngineSubsystem : implements(SObject)
	{
		GENERATED_BODY(SGameEngineSubsystem);

	public:
		SGameEngineSubsystem();
		virtual ~SGameEngineSubsystem() override;

		virtual void Init() {}
		virtual void PostInit() {}
		virtual void Deinit();
		virtual void Tick(float InDeltaTime) {}

	public:
		template<std::derived_from<SGameEngineSubsystem> T>
		static T* Get()
		{
			if (GEngine)
			{
				return GEngine->GetEngineSubsystem<T>();
			}
			return nullptr;
		}
	};
}