// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "TickingGroup.h"

DECLARE_LOG_CATEGORY(GAME_API, LogLevelTick);

namespace libty::inline Game
{
	class SWorld;
	class TickFunction;

	class GAME_API STickTaskLevelManager : implements(SObject)
	{
		GENERATED_BODY(STickTaskLevelManager);

	private:
		struct TickGroupHeader
		{
			ETickingGroup TickGroup;
			std::set<TickFunction*> Functions;

			void AddTickFunction(TickFunction* InFunction);
			void RemoveTickFunction(TickFunction* InFunction);
		};

		SPROPERTY(World)
		SWorld* World = nullptr;
		std::array<TickGroupHeader, (int32)ETickingGroup::NumGroups> TickGroups;

	public:
		STickTaskLevelManager(SWorld* InWorld);

		SWorld* GetWorld();

		void AddTickFunction(TickFunction* InFunction);
		void RemoveTickFunction(TickFunction* InFunction);

	private:
		TickFunction* FrameHead = nullptr;

	public:
		void BeginFrame();
		void IncrementalDispatchTick(ETickingGroup InTickGroup, float InDeltaTime);
		void EndFrame();
	};
}