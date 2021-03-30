// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "TickFunction.h"

#include "Logging/LogCategoryBase.h"
#include "Logging/LogVerbosity.h"

using namespace std;

LogCategoryBase TickFunction::LogTicking(ELogVerbosity::Verbose, L"LogTicking");

TickFunction::TickFunction()
	: bCanEverTick(false)
	, bCompleteTickThisFrame(false)
	, bNeedActualGroup(false)

	, TickGroup(TickingGroup::PrePhysics)
	, TickInterval(0s)
{

}

TickFunction::~TickFunction()
{

}

void TickFunction::ReadyTick()
{
	bCompleteTickThisFrame = false;
	bNeedActualGroup = false;
	ActualTickGroup = TickGroup;

	ComputeTickGroupDependency();
}

void TickFunction::ExecuteTick(Seconds deltaTime)
{
	bCompleteTickThisFrame = true;

	for (auto item : WeakObjectPrerequisites)
	{
		ITickFunctionObject* object = item.Get();
		TickFunction* function = object->GetTickFunction();
		
		if (!function->bCompleteTickThisFrame)
		{
			function->ExecuteTick(deltaTime);
		}
	}
}

void TickFunction::AddPrerequisite(ITickFunctionObject* tickableObject)
{
	WeakObjectPrerequisites.emplace_back(tickableObject);
}

void TickFunction::RemovePrerequisite(ITickFunctionObject* tickableObject)
{
	auto it = find_if(WeakObjectPrerequisites.begin(), WeakObjectPrerequisites.end(), [tickableObject](const auto& ptr)
	{
		return ptr.Get() == tickableObject;
	});
}

void TickFunction::ComputeTickGroupDependency()
{
	size_t remove_cnt = 0;
	for (size_t i = 0; i < WeakObjectPrerequisites.size() - remove_cnt;)
	{
		auto& prer = WeakObjectPrerequisites[i];

		// Remove STALE object references.
		if (!prer.IsValid)
		{
			swap(WeakObjectPrerequisites[i], WeakObjectPrerequisites[WeakObjectPrerequisites.size() - remove_cnt - 1]);
			remove_cnt += 1;
			continue;
		}

		auto ptr = prer.Get();
		TickFunction* function = ptr->GetTickFunction();

		// If compare return less than zero, this tick function will execute
		// after execute tick of prerequisite tickable object.
		if (Compare(TickGroup, function->TickGroup) < 0)
		{
			ActualTickGroup = function->TickGroup;
			bNeedActualGroup = true;
		}

		++i;
	}

	if (remove_cnt != 0)
	{
		WeakObjectPrerequisites.resize(WeakObjectPrerequisites.size() - remove_cnt);
	}
}

int32 TickFunction::Compare(TickingGroup lh, TickingGroup rh)
{
	return Math::Clamp((int32)lh - (int32)rh, -1, 1);
}