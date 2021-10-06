// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Ticking/TickFunction.h"

STickFunction::STickFunction() : Super()
{
}

STickFunction::~STickFunction()
{
}

void STickFunction::Ready()
{
	_bExecutedFrame = false;
	_actualTickGroup = TickGroup;

	ComputeTickGroupDependency();
}

void STickFunction::ExecuteTick(std::chrono::duration<float> deltaTime)
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

void STickFunction::AddPrerequisiteFunction(STickFunction* function)
{
	_pres.emplace_back(function);
}

void STickFunction::RemovePrerequisiteFunction(STickFunction* function)
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

void STickFunction::ComputeTickGroupDependency()
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