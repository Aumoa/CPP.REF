// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickingGroup.h"

DECLARE_LOG_CATEGORY(GAME_API, LogLevelTick);

class SWorld;
class STickFunction;

class GAME_API STickTaskLevelManager : public SObject
{
	GENERATED_BODY(STickTaskLevelManager)

private:
	struct TickGroupHeader
	{
		ETickingGroup TickGroup;
		std::set<STickFunction*> Functions;

		void AddTickFunction(STickFunction* InFunction);
		void RemoveTickFunction(STickFunction* InFunction);
	};

	std::array<TickGroupHeader, (int32)ETickingGroup::NumGroups> _TickGroups;

public:
	STickTaskLevelManager();

	SWorld* GetWorld();

	void AddTickFunction(STickFunction* InFunction);
	void RemoveTickFunction(STickFunction* InFunction);

private:
	STickFunction* _FrameHead = nullptr;

public:
	void BeginFrame();
	void IncrementalDispatchTick(ETickingGroup InTickGroup, float InDeltaTime);
	void EndFrame();
};