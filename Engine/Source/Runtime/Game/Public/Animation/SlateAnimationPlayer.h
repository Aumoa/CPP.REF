// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;
class SSlateAnimationContext;

class GAME_API SSlateAnimationPlayer : implements SObject
{
	GENERATED_BODY(SSlateAnimationPlayer)

private:
	SPROPERTY(Outer)
	SWidget* Outer = nullptr;

	std::map<std::wstring, SSlateAnimationContext*, std::less<>> Animations;
	std::vector<std::wstring> NotifyAnimFinished;

public:
	SSlateAnimationPlayer(SWidget* Outer);
	virtual ~SSlateAnimationPlayer() override;

	void Tick(float InDeltaTime);
	SWidget* GetWidget();

	virtual bool AddAnimation(SSlateAnimationContext* Animation);
	virtual bool RemoveAnimation(SSlateAnimationContext* Animation);
	virtual size_t RemoveAllAnimations();
	virtual SSlateAnimationContext* GetAnimation(std::wstring_view AnimName);

public:
	DECLARE_MULTICAST_EVENT(AnimationFinishedEvent, std::wstring);
	AnimationFinishedEvent AnimationFinished;

private:
	void InvokeAnimFinishedEvents();
};