// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Ticking/TickTaskLevelManager.h"
#include "Level/World.h"

void STickTaskLevelManager::TickGroupHeader::AddTickFunction(STickFunction* InFunction)
{
	Functions.emplace(InFunction);
}

void STickTaskLevelManager::TickGroupHeader::RemoveTickFunction(STickFunction* InFunction)
{
	Functions.erase(InFunction);
}

STickTaskLevelManager::STickTaskLevelManager() : Super()
{
	// Initialize tick group headers.
	for (size_t i = 0; i < _TickGroups.size(); ++i)
	{
		_TickGroups[i].TickGroup = (ETickingGroup)i;
	}
}

SWorld* STickTaskLevelManager::GetWorld()
{
	return Cast<SWorld>(GetOuter());
}

void STickTaskLevelManager::AddTickFunction(STickFunction* InFunction)
{
	// Initialize internal data.
	InFunction->InternalData = std::make_unique<STickFunction::InternalLevelData>();
	InFunction->InternalData->Level = this;
	InFunction->InternalData->NextPtr = nullptr;

	TickGroupHeader& Header = _TickGroups[(int32)InFunction->TickGroup];
	Header.AddTickFunction(InFunction);
}

void STickTaskLevelManager::RemoveTickFunction(STickFunction* InFunction)
{
	TickGroupHeader& Header = _TickGroups[(int32)InFunction->TickGroup];
	Header.RemoveTickFunction(InFunction);
}

void STickTaskLevelManager::BeginFrame()
{
	// Initialize frame.
	_FrameHead = nullptr;

	STickFunction* Tail = nullptr;
	for (auto& Group : _TickGroups)
	{
		ensureMsgf(false, L"TODO: MAKE TICK FUNCTION DEPENDENCY!!");

		for (auto& Function : Group.Functions)
		{
			if (_FrameHead == nullptr)
			{
				_FrameHead = Function;
			}

			if (Tail)
			{
				Tail->InternalData->NextPtr = Function;
				Function->InternalData->NextPtr = nullptr;
			}
			else
			{
				Tail = _FrameHead;
			}
		}
	}
}

void STickTaskLevelManager::IncrementalDispatchTick(ETickingGroup InTickGroup, float InDeltaTime)
{
	for (; _FrameHead && _FrameHead->InternalData->ActualTickGroup == InTickGroup; _FrameHead = _FrameHead->InternalData->NextPtr)
	{
		_FrameHead->ExecuteTick(InDeltaTime);
	}
}

void STickTaskLevelManager::EndFrame()
{
}