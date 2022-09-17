// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "TickingGroup.h"
#include <set>
#include <array>
#include "LevelTick.generated.h"

class World;
class TickFunction;

SCLASS()
class GAME_API LevelTick : public GameObject
{
	GENERATED_BODY();

private:
	struct TickGroupHeader
	{
		ETickingGroup TickGroup;
		std::set<TickFunction*> Functions;

		void AddTickFunction(TickFunction* function);
		void RemoveTickFunction(TickFunction* function);
	};

	std::array<TickGroupHeader, (int32)ETickingGroup::NumGroups> TickGroups;

public:
	LevelTick();

	World* GetWorld() noexcept;

	void AddTickFunction(TickFunction* InFunction);
	void RemoveTickFunction(TickFunction* InFunction);

private:
	TickFunction* FrameHead = nullptr;

public:
	void BeginFrame();
	void IncrementalDispatchTick(ETickingGroup TickGroup, const TimeSpan& DeltaTime);
	void EndFrame();
};