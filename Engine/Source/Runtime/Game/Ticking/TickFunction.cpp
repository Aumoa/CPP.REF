// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "TickFunction.h"

using namespace std;
using namespace std::chrono;

TickFunction::TickFunction() : Super()
{
}

TickFunction::~TickFunction()
{
}

void TickFunction::Ready()
{
	_bExecutedFrame = false;
	_actualTickGroup = TickGroup;

	ComputeTickGroupDependency();
}

void TickFunction::ExecuteTick(duration<float> deltaTime)
{
	_bExecutedFrame = true;

	for (auto& item : _pres)
	{
		if (item->_bExecutedFrame)
		{
			continue;
		}

		item->ExecuteTick(deltaTime);
	}
}

void TickFunction::AddPrerequisiteFunction(TickFunction* function)
{
	_pres.emplace_back(function);
}

void TickFunction::RemovePrerequisiteFunction(TickFunction* function)
{
	if (auto it = find(_pres.begin(), _pres.end(), function); it != _pres.end())
	{
		_pres.erase(it);
	}
}

inline int32 Compare(ETickingGroup lh, ETickingGroup rh)
{
	return MathEx::Clamp<int32>((int32)lh - (int32)rh, -1, 1);
}

void TickFunction::ComputeTickGroupDependency()
{
	_actualTickGroup = TickGroup;
	for (size_t i = 0; i < _pres.size(); ++i)
	{
		auto& function = _pres[i];

		// If compare return less than zero, this tick function will execute
		// after execute tick of prerequisite tickable object.
		if (Compare(TickGroup, function->TickGroup) < 0)
		{
			_actualTickGroup = function->TickGroup;
		}
	}
}