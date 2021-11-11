// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class SWidget;
class SAnimationCurve;

class SLATECORE_API SSlateAnimationContext : implements SObject
{
	GENERATED_BODY(SSlateAnimationContext)

private:
	std::wstring Name;
	std::vector<SAnimationCurve*> AnimCurves;

	std::vector<SAnimationCurve*> QueuedAnimCurves;
	std::vector<SAnimationCurve*> PlayCurves;
	float ElapsedTime;
	uint8 bCurveInit : 1 = false;

public:
	SSlateAnimationContext(std::wstring_view AnimationName);

	virtual std::wstring ToString(std::wstring_view InFormatArgs) override;
	std::wstring GetName();

	bool AddCurve(SAnimationCurve* Curve);
	bool RemoveCurve(SAnimationCurve* Curve);

	void InitCurves();
	void Tick(float InDeltaTime);
	void StopAnimation();

	bool IsPlaying();
	SSlateAnimationContext* Clone();
};