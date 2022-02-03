// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;
class SAnimationCurve;

class GAME_API SSlateAnimationContext : implements SObject
{
	GENERATED_BODY(SSlateAnimationContext)

private:
	std::wstring Name;

	SPROPERTY(AnimCurves)
	std::vector<SAnimationCurve*> AnimCurves;
	SPROPERTY(QueuedAnimCurves)
	std::vector<SAnimationCurve*> QueuedAnimCurves;
	SPROPERTY(PlayCurves)
	std::vector<SAnimationCurve*> PlayCurves;

	float ElapsedTime;
	uint8 bCurveInit : 1 = false;

public:
	SSlateAnimationContext(std::wstring_view AnimationName);

	virtual std::wstring ToString() override;
	std::wstring GetName();

	bool AddCurve(SAnimationCurve* Curve);
	bool RemoveCurve(SAnimationCurve* Curve);

	void InitCurves();
	void Tick(float InDeltaTime);
	void StopAnimation();

	bool IsPlaying();
	SSlateAnimationContext* Clone();
};