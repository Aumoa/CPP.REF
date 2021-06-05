// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Game:TickFunction;

import SC.Runtime.Core;
import std.core;
import :TickingGroup;

using namespace std;
using namespace std::chrono;

/// <summary>
/// Represents tick function that proceed on game engine.
/// </summary>
export class TickFunction : virtual public Object
{
public:
	using Super = Object;

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
	duration<float> TickInterval;

private:
	vector<TickFunction*> _pres;
	ETickingGroup _actualTickGroup = ETickingGroup::PrePhysics;

public:
	/// <summary>
	/// Get actual ticking group.
	/// </summary>
	inline ETickingGroup GetActualTickGroup() const { return _actualTickGroup; }

public:
	/// <summary>
	/// Initialize new <see cref="TickFunction"/> instance.
	/// </summary>
	TickFunction();
	~TickFunction();

	/// <summary>
	/// Reset all states for ready to execute.
	/// </summary>
	virtual void Ready();

	/// <summary>
	/// Execute tick function.
	/// </summary>
	/// <param name="deltaTime"> The frame elapsed time. </param>
	virtual void ExecuteTick(duration<float> deltaTime);

	/// <summary>
	/// Add prerequisite function.
	/// </summary>
	void AddPrerequisiteFunction(TickFunction* function);

	/// <summary>
	/// Remove prerequisite function.
	/// </summary>
	void RemovePrerequisiteFunction(TickFunction* function);

private:
	void ComputeTickGroupDependency();
};