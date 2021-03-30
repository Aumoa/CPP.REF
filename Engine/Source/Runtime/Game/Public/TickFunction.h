// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

struct LogCategoryBase;
struct TickFunction;

enum class TickingGroup
{
	PrePhysics = 0,
	DuringPhysics = 1,
	PostPhysics = 2,
	PostUpdateWork = 3
};

interface GAME_API ITickFunctionObject : virtual public Object
{
	virtual TickFunction* GetTickFunction() = 0;
	virtual void AddPrerequisiteObject(ITickFunctionObject* inObject) = 0;
	virtual void RemovePrerequisiteObject(ITickFunctionObject* inObject) = 0;
};

struct GAME_API TickFunction
{
	using This = TickFunction;

	static LogCategoryBase LogTicking;

	bool bCanEverTick : 1;
	bool bCompleteTickThisFrame : 1;
	bool bNeedActualGroup : 1;

	TickingGroup TickGroup;
	TickingGroup ActualTickGroup;
	Seconds TickInterval;
	std::vector<TWeakPtr<ITickFunctionObject>> WeakObjectPrerequisites;

	TickFunction();
	~TickFunction();

	virtual void ReadyTick();
	virtual void ExecuteTick(Seconds deltaTime);

	void AddPrerequisite(ITickFunctionObject* tickableObject);
	void RemovePrerequisite(ITickFunctionObject* tickableObject);

private:
	void ComputeTickGroupDependency();

	static int32 Compare(TickingGroup lh, TickingGroup rh);
};