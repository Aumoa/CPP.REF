// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Animation/SlateAnimationPlayer.h"
#include "Animation/SlateAnimationContext.h"
#include "Widgets/Widget.h"

SSlateAnimationPlayer::SSlateAnimationPlayer() : Super()
{
}

SSlateAnimationPlayer::~SSlateAnimationPlayer()
{
	InvokeAnimFinishedEvents();
}

void SSlateAnimationPlayer::Tick(float InDeltaTime)
{
	InvokeAnimFinishedEvents();
	std::vector<decltype(Animations.begin())> CompactList;

	for (auto It = Animations.begin(); It != Animations.end(); ++It)
	{
		auto& [Key, Value] = *It;
		Value->Tick(InDeltaTime);

		if (!Value->IsPlaying())
		{
			CompactList.emplace_back(It);
		}
	}

	for (auto& It : CompactList)
	{
		auto& [Key, Value] = *It;
		NotifyAnimFinished.emplace_back(Value->GetName());
		RemoveReferenceObject(Value);
		Animations.erase(It);
	}
}

SWidget* SSlateAnimationPlayer::GetWidget()
{
	return Cast<SWidget>(GetOuter());
}

bool SSlateAnimationPlayer::AddAnimation(SSlateAnimationContext* Animation)
{
	auto [It, bSucceeded] = Animations.emplace(Animation->GetName(), Animation);
	if (!bSucceeded)
	{
		return false;
	}

	AddReferenceObject(Animation);
	Animation->InitCurves();
	return true;
}

bool SSlateAnimationPlayer::RemoveAnimation(SSlateAnimationContext* Animation)
{
	if (auto It = Animations.find(Animation->GetName()); It != Animations.end())
	{
		auto& [Key, Value] = *It;
		Value->StopAnimation();
		NotifyAnimFinished.emplace_back(Value->GetName());
		RemoveReferenceObject(Value);

		Animations.erase(It);
		return true;
	}

	return false;
}

size_t SSlateAnimationPlayer::RemoveAllAnimations()
{
	size_t Count = Animations.size();
	for (auto& [Key, Value] : Animations)
	{
		Value->StopAnimation();
		NotifyAnimFinished.emplace_back(Value->GetName());
		RemoveReferenceObject(Value);
	}
	Animations.clear();
	return Count;
}

SSlateAnimationContext* SSlateAnimationPlayer::GetAnimation(std::wstring_view AnimName)
{
	if (auto It = Animations.find(AnimName); It != Animations.end())
	{
		return It->second;
	}

	return nullptr;
}

void SSlateAnimationPlayer::InvokeAnimFinishedEvents()
{
	for (auto& AnimName : NotifyAnimFinished)
	{
		AnimationFinished.Invoke(AnimName);
	}
	NotifyAnimFinished.clear();
}