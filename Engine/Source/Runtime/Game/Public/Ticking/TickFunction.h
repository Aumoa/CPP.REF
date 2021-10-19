// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickingGroup.h"

class SWorld;
class STickTaskLevelManager;

/// <summary>
/// Represents tick function that proceed on game engine.
/// </summary>
class GAME_API STickFunction : implements SObject
{
	GENERATED_BODY(STickFunction)

public:
	struct InternalLevelData
	{
		STickTaskLevelManager* Level;
		STickFunction* NextPtr;
		ETickingGroup ActualTickGroup;
	};

	std::unique_ptr<InternalLevelData> InternalData;

public:
	uint8 bCanEverTick : 1 = false;
	uint8 bExecutedFrame : 1 = false;

	ETickingGroup TickGroup = ETickingGroup::PrePhysics;
	float TickInterval;
	std::vector<STickFunction*> Prerequisites;

protected:
	STickFunction()
	{
	}

public:
	/// <summary>
	/// Execute tick function.
	/// </summary>
	/// <param name="InDeltaTime"> The frame elapsed time. </param>
	virtual void ExecuteTick(float InDeltaTime) = 0;
};