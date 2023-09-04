// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Bootstrap/BootstrapTask.h"

Spinlock NBootstrapTask::sLock;
std::vector<std::unique_ptr<NBootstrapTask::NContext>> NBootstrapTask::sTasks;

NBootstrapTask::NBootstrapTask(String InName, float Amount)
{
	auto ScopedLock = std::unique_lock(sLock);
	auto& Ptr = sTasks.emplace_back();
	Ptr = std::make_unique<NContext>();

	Context = Ptr.get();
	Context->Name = InName;
	Context->TotalAmount = Amount;
}

NBootstrapTask::~NBootstrapTask() noexcept
{
}

void NBootstrapTask::Step(float InAmount)
{
	float Previous = Context->CurrentAmount;
	Context->CurrentAmount += InAmount;
	if (Context->CurrentAmount > Context->TotalAmount)
	{
		Context->CurrentAmount = Context->TotalAmount;
	}

	if ((int32)Previous == (int32)Context->CurrentAmount)
	{
		return;
	}

	UpdateContext();
}

void NBootstrapTask::Clear()
{
	auto ScopedLock = std::unique_lock(sLock);
	sTasks.clear();
}

void NBootstrapTask::UpdateContext()
{
	String Name;
	float Score = 0;

	{
		auto ScopedLock = std::unique_lock(sLock);

		float TotalScore = 0;
		float CompletedScore = 0;
		for (auto& Task : sTasks)
		{
			PLATFORM_UNREFERENCED_PARAMETER(Task);

			TotalScore += Context->TotalAmount;
			CompletedScore += Context->CurrentAmount;

			float Value = Context->CurrentAmount / Context->TotalAmount;
			if (Value >= Math::SmallNumber && Value <= 1.0f - Math::SmallNumber && Name.IsEmpty())
			{
				Name = Context->Name;
			}
		}

		Score = CompletedScore / TotalScore;
	}

	NGenericSplash::SetSplashText(String::Format(TEXT("Initializing... - {} ({}%)"), Name, (int32)Score));
}