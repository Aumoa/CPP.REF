// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Ticking/TickTaskLevelManager.h"
#include "Level/World.h"

DEFINE_LOG_CATEGORY(LogLevelTick);

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
	if (InFunction->bCanEverTick)
	{
		bool bEnabled = InFunction->bStartWithTickEnabled || InFunction->IsTickFunctionEnabled();
		InFunction->SetTickFunctionEnable(bEnabled);

		// Initialize internal data.
		auto* InternalData = (InFunction->InternalData = std::make_unique<STickFunction::InternalLevelData>()).get();
		InternalData->Level = this;

		InternalData->PrevPtr = nullptr;
		InternalData->NextPtr = nullptr;
		InternalData->ActualTickGroup = InFunction->TickGroup;
		InternalData->TickPriority = 0;

		InternalData->Interval = InFunction->TickInterval;
		InternalData->bTickExecuted = false;

		TickGroupHeader& Header = _TickGroups[(int32)InFunction->TickGroup];
		Header.AddTickFunction(InFunction);
	}
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

	// Sort by tick ordering.
	STickFunction* Tail = nullptr;
	double PriorityCounter = 0;
	for (auto& Group : _TickGroups)
	{
		for (auto& Function : Group.Functions)
		{
			if (Function->IsTickFunctionEnabled())
			{
				if (_FrameHead == nullptr)
				{
					_FrameHead = Function;
				}

				STickFunction::InternalLevelData* InternalData = Function->InternalData.get();

				if (Tail)
				{
					Tail->InternalData->NextPtr = Function;
					InternalData->PrevPtr = Tail;
					InternalData->NextPtr = nullptr;
					Tail = Function;
				}
				else
				{
					Tail = _FrameHead;
					Tail->InternalData->PrevPtr = nullptr;
					Tail->InternalData->NextPtr = nullptr;
				}

				InternalData->TickPriority = PriorityCounter;
				InternalData->bTickExecuted = false;

				PriorityCounter += 1.0;
			}
		}
	}

	// Make dependencies.
	for (auto Head = _FrameHead; Head != nullptr;)
	{
		double MaximumPriority = Head->InternalData->TickPriority;
		STickFunction* MaximumDependency = nullptr;
		for (auto& Dependency : Head->Prerequisites)
		{
			if (Dependency->IsTickFunctionRegistered() && Dependency->IsTickFunctionEnabled())
			{
				double Priority = Dependency->InternalData->TickPriority;
				if (Priority > MaximumPriority)
				{
					MaximumDependency = Dependency;
					MaximumPriority = Priority;
				}
			}
		}

		auto NextHead = Head->InternalData->NextPtr;
		if (MaximumPriority > Head->InternalData->TickPriority)
		{
			// Unlink left and right from head.
			auto InternalHead = Head->InternalData.get();
			if (InternalHead->PrevPtr)
			{
				InternalHead->PrevPtr->InternalData->NextPtr = InternalHead->NextPtr;
			}
			if (InternalHead->NextPtr)
			{
				InternalHead->NextPtr->InternalData->PrevPtr = InternalHead->PrevPtr;
			}

			auto InternalMax = MaximumDependency->InternalData.get();
			InternalHead->NextPtr = InternalMax->NextPtr;
			InternalHead->PrevPtr = MaximumDependency;
			InternalMax->NextPtr = Head;

			if (Head == _FrameHead)
			{
				_FrameHead = MaximumDependency;
			}

			if (InternalHead->ActualTickGroup != InternalMax->ActualTickGroup)
			{
				SE_LOG(LogLevelTick, Warning, L"Actual tick group is different to your desired. It is not an error, but not working as your desired.");
				InternalHead->ActualTickGroup = InternalMax->ActualTickGroup;
			}

			InternalHead->TickPriority = InternalHead->PrevPtr->InternalData->TickPriority;
			if (InternalHead->NextPtr)
			{
				InternalHead->TickPriority += InternalHead->NextPtr->InternalData->TickPriority;
				InternalHead->TickPriority *= 0.5f;
			}
			else
			{
				InternalHead->TickPriority += 1.0f;
			}
		}

		Head = NextHead;
	}
}

void STickTaskLevelManager::IncrementalDispatchTick(ETickingGroup InTickGroup, float InDeltaTime)
{
	for (; _FrameHead && _FrameHead->InternalData->ActualTickGroup == InTickGroup; _FrameHead = _FrameHead->InternalData->NextPtr)
	{
		if (_FrameHead->IsTickFunctionEnabled())
		{
			auto* InternalData = _FrameHead->InternalData.get();
			InternalData->Interval -= InDeltaTime;

			if (InternalData->Interval <= 0.0f)
			{
				_FrameHead->ExecuteTick(InDeltaTime);
				InternalData->bTickExecuted = true;
				InternalData->Interval += _FrameHead->TickInterval;
			}
		}
	}
}

void STickTaskLevelManager::EndFrame()
{
}