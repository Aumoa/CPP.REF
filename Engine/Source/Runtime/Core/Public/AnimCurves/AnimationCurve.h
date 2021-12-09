// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

class CORE_API SAnimationCurve : implements SObject
{
	GENERATED_BODY(SAnimationCurve)

public:
	struct CurveTiming
	{
		float AbsoluteTiming;
		float Timing;
	};

private:
	std::wstring CurveName;
	float CurveStart;
	float CurveEnd;

	float CurveDuration;
	float InvCurveDuration;
	float ElapsedTime;

public:
	SAnimationCurve(std::wstring_view CurveName, float InCurveStart, float InCurveEnd);

	virtual std::wstring ToString() override;
	virtual bool Tick(float InDeltaTime);
	virtual void Reset();

	bool IsEndOfCurve();
	bool IsTimingInRange(float InTiming);
	bool IsTimingInRange(float InStartTiming, float InEndTiming);

	std::wstring GetCurveName();
	float GetCurveDuration();
	float GetCurveStartTime();
	float GetCurveEndTime();

	SAnimationCurve* Clone();

protected:
	virtual void NormalizedTick(CurveTiming CurveTimingStart, CurveTiming CurveTimingEnd) = 0;
	virtual SAnimationCurve* MakeClone() = 0;

private:
	float AdvanceTiming(float InDeltaTime);
	CurveTiming GetCurveTiming(float Timing);
};