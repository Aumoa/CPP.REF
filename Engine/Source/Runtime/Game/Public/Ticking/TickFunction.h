// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickingGroup.h"

class SWorld;
class STickTaskLevelManager;

/// <summary>
/// Represents tick function that proceed on game engine.
/// </summary>
class GAME_API TickFunction
{
public:
	struct InternalLevelData
	{
		STickTaskLevelManager* Level;

		TickFunction* PrevPtr;
		TickFunction* NextPtr;
		ETickingGroup ActualTickGroup;
		double TickPriority;

		float Interval;
		bool bTickExecuted;
	};

	std::unique_ptr<InternalLevelData> InternalData;

public:
	uint8 bCanEverTick : 1 = false;
	uint8 bTickEnabled : 1 = false;
	uint8 bStartWithTickEnabled : 1 = true;

	ETickingGroup TickGroup = ETickingGroup::PrePhysics;
	float TickInterval = 0.0f;
	std::vector<TickFunction*> Prerequisites;

protected:
	TickFunction()
	{
	}

public:
	virtual void ExecuteTick(float InDeltaTime) = 0;

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