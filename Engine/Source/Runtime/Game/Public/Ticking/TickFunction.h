// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickingGroup.h"

class LevelTick;

/// <summary>
/// Represents tick function that proceed on game engine.
/// </summary>
class GAME_API TickFunction
{
public:
	struct InternalLevelData
	{
		LevelTick* Level;

		TickFunction* PrevPtr;
		TickFunction* NextPtr;
		ETickingGroup ActualTickGroup;
		double TickPriority;

		TimeSpan Interval;
		bool bTickExecuted;
	};

	std::unique_ptr<InternalLevelData> InternalData;

public:
	uint8 bCanEverTick : 1 = false;
	uint8 bTickEnabled : 1 = false;
	uint8 bStartWithTickEnabled : 1 = true;

	ETickingGroup TickGroup = ETickingGroup::PrePhysics;
	TimeSpan TickInterval = 0s;
	std::vector<TickFunction*> Prerequisites;

protected:
	TickFunction()
	{
	}

public:
	virtual void ExecuteTick(const TimeSpan& InDeltaTime) = 0;

	void SetTickFunctionEnable(bool bEnabled)
	{
		bTickEnabled = bEnabled;
	}

	bool IsTickFunctionEnabled()
	{
		return bTickEnabled;
	}

	bool IsTickFunctionRegistered()
	{
		return static_cast<bool>(InternalData);
	}
};