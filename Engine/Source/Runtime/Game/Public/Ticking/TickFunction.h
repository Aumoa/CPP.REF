// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <chrono>
#include <vector>
#include "TickingGroup.h"

/// <summary>
/// Represents tick function that proceed on game engine.
/// </summary>
class GAME_API STickFunction : virtual public SObject
{
	GENERATED_BODY(STickFunction)

public:
	/// <summary>
	/// Specify that value represents what this tick function could be never called.
	/// </summary>
	uint8 bCanEverTick : 1 = false;

private:
	uint8 _bExecutedFrame : 1 = false;

public:
	/// <summary>
	/// Represents that tick function was executed this frame.
	/// </summary>
	inline bool IsExecutedOnThisFrame() const { return _bExecutedFrame; }

	/// <summary>
	/// Specify ticking group for this function.
	/// </summary>
	ETickingGroup TickGroup = ETickingGroup::PrePhysics;

	/// <summary>
	/// Tick interval.
	/// </summary>
	std::chrono::duration<float> TickInterval;

private:
	std::vector<STickFunction*> _pres;
	ETickingGroup _actualTickGroup = ETickingGroup::PrePhysics;

public:
	/// <summary>
	/// Get actual ticking group.
	/// </summary>
	inline ETickingGroup GetActualTickGroup() const { return _actualTickGroup; }

public:
	/// <summary>
	/// Initialize new <see cref="STickFunction"/> instance.
	/// </summary>
	STickFunction();
	~STickFunction();

	/// <summary>
	/// Reset all states for ready to execute.
	/// </summary>
	virtual void Ready();

	/// <summary>
	/// Execute tick function.
	/// </summary>
	/// <param name="elapsedTime"> The frame elapsed time. </param>
	virtual void ExecuteTick(std::chrono::duration<float> elapsedTime);

	/// <summary>
	/// Add prerequisite function.
	/// </summary>
	void AddPrerequisiteFunction(STickFunction* function);

	/// <summary>
	/// Remove prerequisite function.
	/// </summary>
	void RemovePrerequisiteFunction(STickFunction* function);

private:
	void ComputeTickGroupDependency();
};