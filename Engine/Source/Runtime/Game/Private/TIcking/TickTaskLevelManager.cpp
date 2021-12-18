// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Ticking/TickTaskLevelManager.h"
#include "Level/World.h"

GENERATE_BODY(STickTaskLevelManager);
DEFINE_LOG_CATEGORY(LogLevelTick);

void STickTaskLevelManager::TickGroupHeader::AddTickFunction(TickFunction* InFunction)
{
	Functions.emplace(InFunction);
}

void STickTaskLevelManager::TickGroupHeader::RemoveTickFunction(TickFunction* InFunction)
{
	Functions.erase(InFunction);
}

STickTaskLevelManager::STickTaskLevelManager(SWorld* InWorld) : Super()
	, World(InWorld)
{
	// Initialize tick group headers.
	for (size_t i = 0; i < TickGroups.size(); ++i)
	{
		TickGroups[i].TickGroup = (ETickingGroup)i;
	}
}

SWorld* STickTaskLevelManager::GetWorld()
{
	return World;
}

void STickTaskLevelManager::AddTickFunction(TickFunction* InFunction)
{
	if (InFunction->bCanEverTick)
	{
		bool bEnabled = InFunction->bStartWithTickEnabled || InFunction->IsTickFunctionEnabled();
		InFunction->SetTickFunctionEnable(bEnabled);

		// Initialize internal data.
		auto* InternalData = (InFunction->InternalData = std::make_unique<TickFunction::InternalLevelData>()).get();
		InternalData->Level = this;

		InternalData->PrevPtr = nullptr;
		InternalData->NextPtr = nullptr;
		InternalData->ActualTickGroup = InFunction->TickGroup;
		InternalData->TickPriority = 0;

		InternalData->Interval = InFunction->TickInterval;
		InternalData->bTickExecuted = false;

		TickGroupHeader& Header = TickGroups[(int32)InFunction->TickGroup];
		Header.AddTickFunction(InFunction);
	}
}

void STickTaskLevelManager::RemoveTickFunction(TickFunction* InFunction)
{
	TickGroupHeader& Header = TickGroups[(int32)InFunction->TickGroup];
	Header.RemoveTickFunction(InFunction);
}

void STickTaskLevelManager::BeginFrame()
{
	// Initialize frame.
	FrameHead = nullptr;

	// Sort by tick ordering.
	TickFunction* Tail = nullptr;
	double PriorityCounter = 0;
	for (auto& Group : TickGroups)
	{
		for (auto& Function : Group.Functions)
		{
			if (Function->IsTickFunctionEnabled())
			{
				if (FrameHead == nullptr)
				{
					FrameHead = Function;
				}

				TickFunction::InternalLevelData* InternalData = Function->InternalData.get();

				if (Tail)
				{
					Tail->InternalData->NextPtr = Function;
					InternalData->PrevPtr = Tail;
					InternalData->NextPtr = nullptr;
					Tail = Function;
				}
				else
				{
					Tail = FrameHead;
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
	for (auto Head = FrameHead; Head != nullptr;)
	{
		double MaximumPriority = Head->InternalData->TickPriority;
		TickFunction* MaximumDependency = nullptr;
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

			if (Head == FrameHead)
			{
				FrameHead = MaximumDependency;
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
	for (; FrameHead && FrameHead->InternalData->ActualTickGroup == InTickGroup; FrameHead = FrameHead->InternalData->NextPtr)
	{
		if (FrameHead->IsTickFunctionEnabled())
		{
			auto* InternalData = FrameHead->InternalData.get();
			InternalData->Interval -= InDeltaTime;

			if (InternalData->Interval <= 0.0f)
			{
				FrameHead->ExecuteTick(InDeltaTime);
				InternalData->bTickExecuted = true;
				InternalData->Interval += FrameHead->TickInterval;
			}
		}
	}
}

void STickTaskLevelManager::EndFrame()
{
}