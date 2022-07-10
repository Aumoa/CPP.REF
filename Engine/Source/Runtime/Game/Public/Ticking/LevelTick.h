// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickingGroup.h"
#include <set>
#include <array>
#include "LevelTick.generated.h"

class World;
class TickFunction;

SCLASS()
class GAME_API LevelTick : virtual public Object
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

	SPROPERTY()
	World* _world = nullptr;
	std::array<TickGroupHeader, (int32)ETickingGroup::NumGroups> _tickGroups;

public:
	LevelTick(World* world);

	World* GetWorld();

	void AddTickFunction(TickFunction* function);
	void RemoveTickFunction(TickFunction* function);

private:
	TickFunction* _frameHead = nullptr;

public:
	void BeginFrame();
	void IncrementalDispatchTick(ETickingGroup tickGroup, float elapsedSeconds);
	void EndFrame();
};