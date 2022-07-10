// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Ticking/LevelTick.h"
#include "Ticking/TickFunction.h"
#include "WorldCore/World.h"
#include "LevelTick.gen.cpp"

static constexpr LogCategory LogLevelTick(TEXT("LogLevelTick"));

void LevelTick::TickGroupHeader::AddTickFunction(TickFunction* function)
{
	Functions.emplace(function);
}

void LevelTick::TickGroupHeader::RemoveTickFunction(TickFunction* function)
{
	Functions.erase(function);
}

LevelTick::LevelTick(World* world) : Super()
	, _world(world)
{
	// Initialize tick group headers.
	for (size_t i = 0; i < _tickGroups.size(); ++i)
	{
		_tickGroups[i].TickGroup = (ETickingGroup)(int32)i;
	}
}

World* LevelTick::GetWorld()
{
	return _world;
}

void LevelTick::AddTickFunction(TickFunction* function)
{
	if (function->bCanEverTick)
	{
		bool bEnabled = function->bStartWithTickEnabled || function->IsTickFunctionEnabled();
		function->SetTickFunctionEnable(bEnabled);

		// Initialize internal data.
		auto* internalData = (function->InternalData = std::make_unique<TickFunction::InternalLevelData>()).get();
		internalData->Level = this;

		internalData->PrevPtr = nullptr;
		internalData->NextPtr = nullptr;
		internalData->ActualTickGroup = function->TickGroup;
		internalData->TickPriority = 0;

		internalData->Interval = function->TickInterval;
		internalData->bTickExecuted = false;

		TickGroupHeader& header = _tickGroups[(int32)function->TickGroup];
		header.AddTickFunction(function);
	}
}

void LevelTick::RemoveTickFunction(TickFunction* function)
{
	TickGroupHeader& header = _tickGroups[(int32)function->TickGroup];
	header.RemoveTickFunction(function);
}

void LevelTick::BeginFrame()
{
	// Initialize frame.
	_frameHead = nullptr;

	// Sort by tick ordering.
	TickFunction* tail = nullptr;
	double priorityCounter = 0;
	for (auto& group : _tickGroups)
	{
		for (auto& Function : group.Functions)
		{
			if (Function->IsTickFunctionEnabled())
			{
				if (_frameHead == nullptr)
				{
					_frameHead = Function;
				}

				TickFunction::InternalLevelData* internalData = Function->InternalData.get();

				if (tail)
				{
					tail->InternalData->NextPtr = Function;
					internalData->PrevPtr = tail;
					internalData->NextPtr = nullptr;
					tail = Function;
				}
				else
				{
					tail = _frameHead;
					tail->InternalData->PrevPtr = nullptr;
					tail->InternalData->NextPtr = nullptr;
				}

				internalData->TickPriority = priorityCounter;
				internalData->bTickExecuted = false;

				priorityCounter += 1.0;
			}
		}
	}

	// Make dependencies.
	for (auto head = _frameHead; head != nullptr;)
	{
		double maximumPriority = head->InternalData->TickPriority;
		TickFunction* maximumDependency = nullptr;
		for (auto& dependency : head->Prerequisites)
		{
			if (dependency->IsTickFunctionRegistered() && dependency->IsTickFunctionEnabled())
			{
				double priority = dependency->InternalData->TickPriority;
				if (priority > maximumPriority)
				{
					maximumDependency = dependency;
					maximumPriority = priority;
				}
			}
		}

		auto nextHead = head->InternalData->NextPtr;
		if (maximumPriority > head->InternalData->TickPriority)
		{
			// Unlink left and right from head.
			auto internalHead = head->InternalData.get();
			if (internalHead->PrevPtr)
			{
				internalHead->PrevPtr->InternalData->NextPtr = internalHead->NextPtr;
			}
			if (internalHead->NextPtr)
			{
				internalHead->NextPtr->InternalData->PrevPtr = internalHead->PrevPtr;
			}

			auto internalMax = maximumDependency->InternalData.get();
			internalHead->NextPtr = internalMax->NextPtr;
			internalHead->PrevPtr = maximumDependency;
			internalMax->NextPtr = head;

			if (head == _frameHead)
			{
				_frameHead = maximumDependency;
			}

			if (internalHead->ActualTickGroup != internalMax->ActualTickGroup)
			{
				Log::Warning(LogLevelTick, TEXT("Actual tick group is different to your desired. It is not an error, but not working as your desired."));
				internalHead->ActualTickGroup = internalMax->ActualTickGroup;
			}

			internalHead->TickPriority = internalHead->PrevPtr->InternalData->TickPriority;
			if (internalHead->NextPtr)
			{
				internalHead->TickPriority += internalHead->NextPtr->InternalData->TickPriority;
				internalHead->TickPriority *= 0.5f;
			}
			else
			{
				internalHead->TickPriority += 1.0f;
			}
		}

		head = nextHead;
	}
}

void LevelTick::IncrementalDispatchTick(ETickingGroup tickGroup, float elapsedSeconds)
{
	for (; _frameHead && _frameHead->InternalData->ActualTickGroup == tickGroup; _frameHead = _frameHead->InternalData->NextPtr)
	{
		if (_frameHead->IsTickFunctionEnabled())
		{
			auto* internalData = _frameHead->InternalData.get();
			internalData->Interval -= elapsedSeconds;

			if (internalData->Interval <= 0.0f)
			{
				_frameHead->ExecuteTick(elapsedSeconds);
				internalData->bTickExecuted = true;
				internalData->Interval += _frameHead->TickInterval;
			}
		}
	}
}

void LevelTick::EndFrame()
{
}