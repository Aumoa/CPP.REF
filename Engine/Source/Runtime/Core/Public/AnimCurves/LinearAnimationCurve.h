// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimationCurve.h"

template<class T>
class SLinearAnimationCurve : public SAnimationCurve
{
	GENERATED_BODY(SLinearAnimationCurve, T)

private:
	T MyTarget;
	const T StartValue;
	const T EndValue;

	T ScopeDelta;
	T* BoundTarget = nullptr;

public:
	SLinearAnimationCurve(const T& StartValue, const T& EndValue, float InCurveStart, float InCurveEnd)
		: Super(L"LinearAnimationCurve", InCurveStart, InCurveEnd)
		, StartValue(StartValue)
		, EndValue(EndValue)
	{
		ScopeDelta = EndValue - StartValue;
	}

	SLinearAnimationCurve(T& CurveTarget, const T& StartValue, const T& EndValue, float InCurveStart, float InCurveEnd)
		: This(StartValue, EndValue, InCurveStart, InCurveEnd)
	{
		BindTarget(CurveTarget);
	}

	const T& GetTiming()
	{
		return MyTarget;
	}

	void BindTarget(T& CurveTarget)
	{
		check(BoundTarget == nullptr);
		BoundTarget = &CurveTarget;
	}

protected:
	virtual void NormalizedTick(CurveTiming CurveTimingBegin, CurveTiming CurveTimingEnd) override
	{
		float Timing = CurveTimingEnd.Timing;
		MyTarget = StartValue + ScopeDelta * Timing;

		if (BoundTarget)
		{
			*BoundTarget = MyTarget;
		}
	}

	virtual SAnimationCurve* MakeClone() override
	{
		return NewObject<SLinearAnimationCurve>(StartValue, EndValue, GetCurveStartTime(), GetCurveEndTime());
	}
};